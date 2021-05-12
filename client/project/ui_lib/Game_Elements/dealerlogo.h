#ifndef DEALERLOGO_H
#define DEALERLOGO_H

#include <QPixmap>
#include <QLabel>
#include <QImage>

class DealerLogo : public QLabel {
public:
    DealerLogo();
    ~DealerLogo();
private:
    QImage *mypix;
    QImage logo;
};

#endif // DEALERLOGO_H
