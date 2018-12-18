#include "mainwindow.h"

#include <string>
#include <iostream>
#include <QIcon>       // for QIcon
#include <QMenu>       // for QMenu
#include <QScreen>
#include <QAction>     // for QAction
#include <QMenuBar>    // for menuBar()
#include <QToolBar>    // for QToolBar, addToolBar()
#include <QMessageBox>
#include <QFileDialog>
#include <QImageWriter>
#include <QImageReader>
#include <QGridLayout>
#include "image/image.h"


namespace gui {

MainWindow::MainWindow(Client &client, QWidget *parent)
  : client_(client), QMainWindow(parent) {
  compressedFilename = QString::fromStdString("/tmp/compressed.pgm");
  setupGui();
}

MainWindow::~MainWindow(){}

//=============================================================================
// static functions

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode) {
  static bool firstDialog = true;
  if (firstDialog) {
    firstDialog = false;
  }

  QStringList mimeTypeFilters;
  const QByteArrayList supportedMimeTypes = (acceptMode ==  QFileDialog::AcceptOpen) ?
    QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();

  foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
    mimeTypeFilters.append(mimeTypeName);

  mimeTypeFilters.sort();
  dialog.setMimeTypeFilters(mimeTypeFilters);
  dialog.selectMimeTypeFilter("image/pgm");
  if (acceptMode == QFileDialog::AcceptSave)
    dialog.setDefaultSuffix("pgm");
}

//=============================================================================
// private methods

bool MainWindow::loadFile(const QString &filename, bool compressed) {
  QImageReader reader(filename);
  const QImage newImage = reader.read();
  if (newImage.isNull()) {
    QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                             tr("Cannot load %1, %2").arg(QDir::toNativeSeparators(filename), reader.errorString()));
    return false;
  }

  if (compressed) {
    setCompressedImage(filename, newImage);
  } else {
    setImage(filename, newImage);
    setWindowFilePath(filename);
  }

  const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
    .arg(QDir::toNativeSeparators(filename)).arg(image.width()).arg(image.height()).arg(image.depth());
  statusBar()->showMessage(message, 3000);
  return true;
}

void MainWindow::setImage(const QString &filename, const QImage &newImage) {
  image = newImage;
  imageFilename = filename;
  imageLabel->setPixmap(QPixmap::fromImage(image));
  imageLabel->resize(imageLabel->pixmap()->size());
}

void MainWindow::setCompressedImage(const QString &filename, const QImage &newImage) {
  compressedImage = newImage;
  compressedFilename = filename;
  compressedImageLabel->setPixmap(QPixmap::fromImage(compressedImage));
  compressedImageLabel->resize(compressedImageLabel->pixmap()->size());
}

bool MainWindow::saveFile(const QString &filename) {
  QImageWriter writer(filename);
  if (!writer.write(compressedImage)) {
    QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                             tr("Cannot write %1, %2").arg(QDir::toNativeSeparators(filename), writer.errorString()));
    return false;
  }

  const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(filename));
  statusBar()->showMessage(message, 3000);
  return true;
}

//=============================================================================
// slots

void MainWindow::open() {
  QFileDialog dialog(this, tr("Open File"));
  initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);
  while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first(), false)) {}
}

void MainWindow::compress() {
  if (!image.isNull()) {
    compressBtn->setEnabled(false);

    // read image intensities
    vector<vector<int>> intensities;
    intensities.resize(image.height());
    for (int i = 0; i < image.height(); i++) {
      for (int j = 0; j < image.width(); j++) {
        intensities[i].push_back(image.pixelColor(j, i).value());
      }
    }
    statusBar()->showMessage(QString::fromStdString("compressing " + imageFilename.toStdString() + "..."), 30000);

    // create custom image class and compress data
    Image img(imageFilename.toStdString(), image.height(), image.width(), intensities);
    unsigned int ratio = ratioLabel->text().toInt();
    img.compress(ratio, true);

    // perform huffman encoding
    statusBar()->showMessage(QString::fromStdString("Performing huffman encoding and sending over network..."), 5000);
    vector<vector<bool>> HuffmanVec = img.HuffmanEncode();
    vector<bool> flattened;
    for (auto row : HuffmanVec)
      for (bool b : row)
        flattened.push_back(b);

    // send over the network
    client_.write(flattened);
    statusBar()->showMessage(QString::fromStdString("Succesfully compressed and sent image!"), 2000);

    // save the compressed image as a temporary file to display result
    img.decompress(true);
    img.saveImage(compressedFilename.toStdString(), true);
    statusBar()->showMessage(QString::fromStdString("Displaying compresed image..."), 2000);

    //vector<vector<int>> compressed = img.getCompressedIntensities();
    //QImage newImage(image.width(), image.height(), image.format());
    //for (int i = 0; i < image.height(); i++) {
    //  for (int j = 0; j < image.width(); j++) {
    //    newImage.setPixel(j, i, compressed[i][j]);
    //  }
    //}
    //newImage.save(compressedFilename);

    loadFile(compressedFilename, true);
    compressBtn->setEnabled(true);
  }
}

void MainWindow::saveAs() {
  QFileDialog dialog(this, tr("Save File As"));
  initializeImageFileDialog(dialog, QFileDialog::AcceptSave);
  while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().first())) {}
}

//=============================================================================
// Widget Creation

bool MainWindow::setupGui() {
  // create a container widget, and give it the grid layout
  QWidget *container = new QWidget(this);
  setCentralWidget(container);

  // create a gridlayout to contain all the elements
  QGridLayout *grid = new QGridLayout();
  grid->setSpacing(5);

  // create the image label for viewing original images
  imageLabel = new QLabel();
  imageLabel->setBackgroundRole(QPalette::Base);
  imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  imageLabel->setScaledContents(true);
  grid->addWidget(imageLabel, 1, 1, 3, 3);

  // create the compression ratio slider
  ratioLabel = new QLabel("0", this);
  slider = new QSlider(Qt::Horizontal, this);
  connect(slider, &QSlider::valueChanged, ratioLabel, static_cast<void (QLabel::*)(int)>(&QLabel::setNum));
  grid->addWidget(slider, 5, 1, 1, 2);
  grid->addWidget(ratioLabel, 5, 3, 1, 1);

  // create button for opening image
  QPushButton *openImageBtn = new QPushButton("Open Image", container);
  openImageBtn->setMinimumSize(1, 35);
  connect(openImageBtn, &QPushButton::clicked, this, &MainWindow::open);
  grid->addWidget(openImageBtn, 6, 1, 1, 1);

  // create button for performing compression
  compressBtn = new QPushButton("Compress Image", container);
  compressBtn->setMinimumSize(1, 35);
  connect(compressBtn, &QPushButton::clicked, this, &MainWindow::compress);
  grid->addWidget(compressBtn, 6, 2, 1, 1);

  // create label for viewing the compressed image
  compressedImageLabel = new QLabel();
  compressedImageLabel->setBackgroundRole(QPalette::Base);
  compressedImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  compressedImageLabel->setScaledContents(true);
  grid->addWidget(compressedImageLabel, 1, 5, 3, 3);

  QPushButton *saveBtn = new QPushButton("Save Compressed Image", container);
  saveBtn->setMinimumSize(1, 35);
  connect(saveBtn, &QPushButton::clicked, this, &MainWindow::saveAs);
  grid->addWidget(saveBtn, 6, 5, 1, 1);

  // create a status bar
  statusBar();

  // create the menu
  createMenu();

  // setup application
  container->setLayout(grid);
  resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
  setWindowTitle("Parallel Compression");
}

void MainWindow::createMenu() {
  // load the icons that will be used in the menu
  QPixmap savePix("../images/icons/save.png");
  QPixmap openPix("../images/icons/open.png");
  QPixmap quitPix("../images/icons/quit.jpeg");

  QMenu *file;
  file = menuBar()->addMenu("&File");

  QAction *openAction = file->addAction(openPix, tr("&Open"), this, &MainWindow::open);
  openAction->setShortcut(QKeySequence::Open);

  QAction *saveAsAction = file->addAction(savePix, tr("&Save Compressed"), this, &MainWindow::saveAs);
  saveAsAction-> setEnabled(true);

  file->addSeparator();
  QAction *quit = file->addAction(quitPix, tr("&Quit"), this, &QApplication::quit);
  quit->setShortcut(tr("CTRL+Q"));

  qApp->setAttribute(Qt::AA_DontShowIconsInMenus, false);
}

}  // namespace gui
