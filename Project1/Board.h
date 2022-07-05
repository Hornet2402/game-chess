#pragma once
#include "Figures.h"
#include "ChoosePiece.h"
namespace Project1 {
	using namespace System;
	using namespace System::Windows::Forms;
	using namespace System::Drawing;

	ref class Board {
	public:
		ref class Cell {
		public:
			int x;
			int y;
			Color color;
			Figure^ figure = nullptr;
			Cell(int x, int y, Figure^ figure) {
				this->x = x;
				this->y = y;
				this->figure = figure;
				if ((x + y) % 2) {
					color = Color::FromArgb(209, 139, 71);
				}
				else {
					color = Color::FromArgb(255, 206, 158);
				}
			}
			void Draw(Graphics^ graphics) {
				Brush^ brush;
				brush = gcnew SolidBrush(color);
				int length = SIZE / 6;
				graphics->FillRectangle(brush, x * length, y * length, length, length);
				if (figure != nullptr) {
					figure->Draw(graphics, x, y, length);
				}
			}
			bool Move(int new_x, int new_y) {
				if (figure == nullptr) {
					return false;
				}
				array<int, 2>^ int_cells = gcnew array<int, 2>(6, 6);
				for (int i = 0; i < 6; i++) {
					for (int j = 0; j < 6; j++) {
						if (cells[i, j]->figure == nullptr) {
							int_cells[i, j] = -1;
						}
						else if (cells[i, j]->figure->colour == true) {
							int_cells[i, j] = 1;
						}
						else {
							int_cells[i, j] = 0;
						}
					}
				}
				bool res = figure->Move(x, y, new_x, new_y, int_cells);
				return res;
			}
			void ResetColor() {
				if ((x + y) % 2) {
					color = Color::FromArgb(209, 139, 71);
				}
				else {
					color = Color::FromArgb(255, 206, 158);
				}
			}
		};
		static const int SIZE = 450;
		PictureBox^ picturebox;
		static array< Cell^, 2>^ cells;
		bool move;
		bool gameOver = false;
	public:
		Board(PictureBox^ picturebox) {
			this->picturebox = picturebox;
			move = true;
			cells = gcnew array<Cell^, 2>(6, 6);
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 6; j++) {
					cells[i, j] = gcnew Cell(i, j, nullptr);
				}
			}
			for (int i = 0; i < 6; i++) {
				cells[i, 1]->figure = gcnew Pawn(false);
				cells[i, 4]->figure = gcnew Pawn(true);
			}
			cells[0, 0]->figure = gcnew Rook(false);
			cells[5, 0]->figure = gcnew Rook(false);
			cells[0, 5]->figure = gcnew Rook(true);
			cells[5, 5]->figure = gcnew Rook(true);
			cells[4, 0]->figure = gcnew Knight(false);
			cells[1, 0]->figure = gcnew Knight(false);
			cells[1, 5]->figure = gcnew Knight(true);
			cells[4, 5]->figure = gcnew Knight(true);
			cells[2, 0]->figure = gcnew Queen(false);
			cells[2, 5]->figure = gcnew Queen(true);
			cells[3, 0]->figure = gcnew King(false);
			cells[3, 5]->figure = gcnew King(true);
			Draw();
		}
		void Draw() {
			Bitmap^ bitmap = gcnew Bitmap(SIZE, SIZE);
			Graphics^ graphics = Graphics::FromImage(bitmap);
			graphics->Clear(picturebox->BackColor);
			int length = SIZE / 6;
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 6; j++) {
					cells[i, j]->Draw(graphics);
				}
			}
			
			picturebox->Image = bitmap;
		}
		bool IsFigureOnCell(int x, int y) {
			int length = SIZE / 6;
			return cells[x / length, y / length]->figure != nullptr;
		}
		bool Move(int x, int y, int new_x, int new_y) {
			int length = SIZE / 6;
			x = x / length;
			y = y / length;
			new_x /= length;
			new_y /= length;
			if (cells[x, y]->figure->colour != move) {
				return false;
			}
			if (x == new_x && y == new_y) {
				return true;
			}
			if (cells[x, y]->Move(new_x, new_y )) {
				if (cells[new_x, new_y]->figure != nullptr &&
					cells[new_x, new_y]->figure->GetType() == King::typeid) {
					cells[new_x, new_y]->figure = cells[x, y]->figure;
					cells[x, y]->figure = nullptr;
					Draw();
					MessageBox::Show("Game over");
					

					gameOver = true;
					return true;
				}
				cells[new_x, new_y]->figure = cells[x, y]->figure;
				cells[x, y]->figure = nullptr;
				if (cells[new_x, new_y]->figure->GetType() == Pawn::typeid) {
					bool end = false;
					if (cells[new_x, new_y]->figure->colour == true && new_y == 0) {
						end = true;
					}
					if (cells[new_x, new_y]->figure->colour == false && new_y == 5) {
						end = true;
					}
					if (end) {
						ChoosePiece^ form = gcnew ChoosePiece();
						bool colour = cells[new_x, new_y]->figure->colour;
						form->ShowDialog();
						if (form->ret == "Queen") {
							cells[new_x, new_y]->figure = gcnew Queen(colour);
						}
						if (form->ret == "Rook") {
							cells[new_x, new_y]->figure = gcnew Rook(colour);
						}
						if (form->ret == "Knight") {
							cells[new_x, new_y]->figure = gcnew Knight(colour);
						}
					}
				}
				Draw();
				move = !move;
				return true;
			}
			return false;
		}
		void ShowPossibleTurns(int x, int y) {
			int length = SIZE / 6;
			x /= length;
			y /= length;
			if (cells[x, y]->figure->colour != move) {
				return;
			}
			array<int, 2>^ int_cells = gcnew array<int, 2>(6, 6);
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 6; j++) {
					if (cells[i, j]->figure == nullptr) {
						int_cells[i, j] = -1;
					}
					else if (cells[i, j]->figure->colour == true) {
						int_cells[i, j] = 1;
					}
					else {
						int_cells[i, j] = 0;
					}
				}
			}
			array<bool, 2>^ highlight = cells[x, y]->figure->ShowPossibleTurns(x, y, int_cells);
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 6; j++) {
					if (highlight[i, j] == true) {
						cells[i, j]->color = Color::Red;
					}
				}
			}
			Draw();
		}
		void HidePossibleTurns() {
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 6; j++) {
					cells[i, j]->ResetColor();
				}
			}
			Draw();
		}
	};
}