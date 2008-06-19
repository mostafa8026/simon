//
// C++ Interface: icondialog
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ICONDIALOG_H
#define  ICONDIALOG_H

#include <QDialog>

class QListView;
class QIcon;
class QLineEdit;
class QPushButton;
class QRadioButton;
class QStandardItemModel;

/**
	@author Susanne Tschernegg
*/
/**
 * \class IconDialog
 * \author Susanne Tschernegg
 * \date 13.12.2007
 * \version 0.1
 * \brief This dialog is called, when the iconbutton is clicked. The user can choose an icon from a local file or a system-icon.
 */
class IconDialog : public QDialog
{
    Q_OBJECT

private:
#ifdef __WIN32
    QListView *iconView;
    QStandardItemModel *model;
    QRadioButton *rbIconFromFile;
    QRadioButton *rbIconView;
#endif
    QString resourceIdStr;
    QLineEdit *leFromFile;
    QPushButton *pbFromFile;
    
public:
    IconDialog(QWidget *parent = 0);
    ~IconDialog();

    void showIcons(QString currentIcon = "");
    QString getIcon(){return resourceIdStr;}

private slots:
    void safeIconInformation();
    void enableSystemIcons();
    void enableLineEdit();
    void openFileDialog();

signals:
    void iconSelected(QString resourceId);
};

#endif