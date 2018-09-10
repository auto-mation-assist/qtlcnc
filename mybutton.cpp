#include "mybutton.h"

MyButton::MyButton(QStringList & sl, LayoutData* ld, int init_state=-1)
{
    elements = sl;
    ld_local = ld;
    // parse all data and fill the structures for button to speed up drawing afterwards
    // setup default state
    current_state = -1;
    if (elements.size() > 0) current_state = 0;
    if ( (init_state < elements.size()) && (init_state >= 0)) current_state = init_state;

}

bool MyButton::setState(int state){
    if ( (state >= 0) && (state < elements.size())) {
        current_state = state;
        return true;
    }
    return false;
}

void MyButton::drawLayoutObject(QPainter &painter, QPoint& loffset){
// get actual data from the structure and do actual drawing of the button
    // 1) get current state
    if (current_state < 0 ){
        qDebug() << "button has no states, fix ini file";
        return;
    }
    // check if the element should appear on layout
    if (this->is_visible() && ld_local != NULL){
        //get the state configuration
        MyState* cs = ld_local->get_state_object_by_name( elements [current_state] );
        //qDebug() << "got state " <<  elements [current_state] ;
        QPoint * pt = cs->get_Position(ld_local);
        //qDebug() << "pt x= " << pt->x() << "pt y = " << pt->y();
        QPoint * sz = cs->get_Size(ld_local);
        //qDebug() << "size x= " << sz->x() << "size y = " << sz->y();
        // get the unit size
        QString us = QString("ucell");
        int ucell = ld_local->get_int_value_by_name(us);
        //qDebug() << "ucell= " << ucell;

        //loop over all pathes
        int path_num = cs->get_paths_number();
        for (int i=0; i<path_num; i++) {
            QString bgcolor = cs->get_bgColor(ld_local, i);
            //qDebug() << "bgcol = " << bgcolor;
            QString pencolor = cs->get_pColor(ld_local, i);
            //qDebug() << "pcol = " << pencolor;

            // get PIX
            Path2D * pix = cs->get_Pix(ld_local, i);

            pix->drawPath(painter, pencolor, bgcolor, pt, sz, ucell, loffset );
        }
    }
}

bool MyButton::isCursorOver(QPoint& pos){
    if (current_state < 0 ){
        qDebug() << "button has no states, fix ini file";
        return false;
    }
    if (this->is_visible() && ld_local != NULL){
        //qDebug() << "button is visible";
        //get the state configuration
        MyState* cs = ld_local->get_state_object_by_name( elements [current_state] );
        //qDebug() << "button state =" << cs;
        QPoint * pt = cs->get_Position(ld_local);
        //qDebug() << "pt x= " << pt->x() << "pt y = " << pt->y();
        QPoint * sz = cs->get_Size(ld_local);
        //qDebug() << "size x= " << sz->x() << "size y = " << sz->y();
        QString ustr = QString("ucell");
        int ucell = ld_local->get_int_value_by_name(ustr);
        if ( (pos.x() >= (pt->x()-1)*ucell) && (pos.x() < (pt->x()+sz->x()-1) *ucell) &&
             (pos.y() >= (pt->y()-1)*ucell) && (pos.y() < (pt->y()+sz->y()-1) *ucell) )
            return true;
    }
    return false;
}
// as soon as button can have dofferent states, the command sent by current state could be different.
QString MyButton::get_active_state_command(){
    if (current_state < 0 ){
        qDebug() << "button has no states, fix ini file";
        return QString();
    }
    if (this->is_visible() && ld_local != NULL){
        //get the state configuration
        MyState* cs = ld_local->get_state_object_by_name( elements [current_state] );
        QString cmd = cs->get_command(ld_local);
        cmd.remove(cmd.size()-1,1);
        cmd.remove(0,1);
        return cmd;
    }
    return QString();
}

//
QString MyButton::get_update_group_command(){
    if (current_state < 0 ){
        qDebug() << "button has no states, fix ini file";
        return QString();
    }
    if (this->is_visible() && ld_local != NULL){
        //get the state configuration
        MyState* cs = ld_local->get_state_object_by_name( elements [current_state] );
        QString cmd = cs->get_update_group_command(ld_local);
        cmd.remove(cmd.size()-1,1);
        cmd.remove(0,1);
        return cmd;
    }
    return QString();
}
// to define later
void MyButton::selectLayoutObject(){}
void MyButton::setLayoutObjectStatus(){}
void MyButton::getLayoutObjectStatus(){}
void MyButton::setLayoutObjectPropertiy(){}
void MyButton::getLayoutObjectPropertiy(){}

