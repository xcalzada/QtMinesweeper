#include <QtWidgets/QApplication>
#include "minesweeper.h"
#include <QPoint>
#include <QGraphicsScene>
#include <QMouseEvent>


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	minesweeper mine;
		
	mine.setWindowTitle("Minesweeper"); 
	mine.show();

	return app.exec();
}

