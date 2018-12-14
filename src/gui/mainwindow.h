#pragma once

#include <vector>
#include <QLabel>
#include <QImage>
#include <QSlider>
#include <QStatusBar>
#include <QPushButton>
#include <QMainWindow>
#include <QApplication>
#include <boost/asio.hpp>
#include "client/client.h"

using namespace client;
using std::vector;

namespace gui {

/// The Main window of the client application.
/// In order to work with menus, we must inherit from a QMainWindow widget.
class MainWindow : public QMainWindow {

  // allows for creation of custom slots/events
  Q_OBJECT

  public:
    //= Constructor ===========================================================
    /// Constructs the MainWindow with possible parent widget.
    MainWindow(Client &client, QWidget *parent = 0);

    /// Destructs the MainWindow
    ~MainWindow();

  private slots:
    //= Slots =================================================================):
    /// Compress the current image.
    void compress();

    /// Allows the user to open a new image file.
    void open();

    /// Allows the user to save the compressed image file.
    void saveAs();

  private:
    //= Private Helpers ======================================================
    /// Loads the specified file to be viewed in the GUI.
    /// The compressed flag specifies whether the image file should displayed
    /// as the original or compressed image.
    bool loadFile(const QString &filename, bool compressed);

    /// Creates the menu in the top left of the application
    void createMenu();

    /// Sets the current image that is being displayed.
    void setImage(const QString &filename, const QImage &newImage);

    /// Sets the compressed image that is being displayed.
    void setCompressedImage(const QString &filename, const QImage &newImage);

    /// Saves the current image file as the specified filename.
    bool saveFile(const QString &filename);

    /// Saves the compression algorithm
    void compressionCallback();

    /// Sets up the GUI Application
    bool setupGui();

    //= Member Variables ======================================================
    QLabel *ratioLabel;        // label for displaying compression ratio
    QSlider *slider;           // slider for choosing compression ratio
    QPushButton *compressBtn;  // button for running compression

    QImage image;             // reference to original image
    QLabel *imageLabel;       // label for displaying original image
    QString imageFilename;    // name fo the image file

    QImage compressedImage;        // reference to compressed image
    QLabel *compressedImageLabel;  // label for displaying compressed image
    QString compressedFilename;    // name fo the compressed image file

    Client &client_;
};

}  // namespace gui
