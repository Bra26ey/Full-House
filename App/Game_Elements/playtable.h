#ifndef PLAYTABLE_H
#define PLAYTABLE_H

#include <QWidget>
#include <QPixmap>

class PlayTable:public QWidget {
public:
    PlayTable(size_t texture_id);
    ~PlayTable();

private:
    size_t mTextureId;
    QPixmap *mTexture;

};

#endif // PLAYTABLE_H
