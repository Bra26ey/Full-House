#include "basefragment.h"
#include <QVBoxLayout>

BaseFragment::BaseFragment() {}
BaseFragment::~BaseFragment() {}
void BaseFragment::onPause() {}
void BaseFragment::onResume() {}

void BaseFragment::clearList(QLayout *list) {
    QLayoutItem* child;
    while(list->count()!=0)
    {
        child = list->takeAt(0);
        if(child->layout() != 0)
        {
            clearList(child->layout());
        }
        else if(child->widget() != 0)
        {
            delete child->widget();
        }
        delete child;
    }
}
