#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "stockitemlistmodel.h"
#include <QMainWindow>

namespace Ui {
	class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

// Example of slot definition in mainwindow.h---------------------------
	public slots:
		void handleAddButton();
		void handleEditButton();
		void handleRemoveButton();
		void on_actionSave_triggered();
// ---------------------------------------------------------------------

	signals:
		void statusUpdateMessage( const QString & message, int timeout );


	public:
    		explicit MainWindow(QWidget *parent = 0);
    		~MainWindow();


	private:
		StockItemListModel stockList;
    		Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
