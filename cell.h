#pragma once
#include <QtWidgets/QLabel>

class cell : public QLabel {
	Q_OBJECT

private:
	bool opened = false;
	bool empty = true;

	int state = 0;
	int rightClick = 0;
	int posX = 0;
	int posY = 0;

	cell *up = nullptr;
	cell *down = nullptr;
	cell *right = nullptr;
	cell *left = nullptr;

	/*!
	 * Check if the cells around current cell are opened or not
	 * If the current cell is blank will open the around cells
	 */
	void cellOp();
	void checkCells();
	int isOpened();

public:
	cell(QWidget *parent = 0);
	virtual ~cell();

	/*!
	 * Control if the game is finalised
	 * 
	 */
	static bool endGame;

	/*!
	 * Count number of the flag in on the board
	 * 
	 */
	static int countFlagBomb;

	const int getState() const;

	void setState(int) ;
	void setImage(bool);

	void setUp(cell*);
	void setDown(cell*);
	void setRight(cell*);
	void setLeft(cell*);

	void setEndGame(bool);
	void resetCountFlagBomb();

	int eventCell(QMouseEvent *ev);
};
