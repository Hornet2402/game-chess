#pragma once
#include "Board.h";
#include <math.h>;

namespace Project1 {
	using namespace System;
	using namespace System::Windows::Forms;
	using namespace System::Drawing;

	ref class Figure abstract {
	protected: 
		String^ path;
	public:
		bool colour;
		Figure() {
			colour = true;
			path = "";
		}
		Figure(bool colour) {
			this->colour = colour;
		}
		void Draw(Graphics^ graphics, int x, int y, int length);
		virtual bool Move(int x, int y, int new_x, int new_y, array<int, 2>^ cells) = 0;
		array<bool, 2>^ ShowPossibleTurns(int x, int y, array<int, 2>^ cells) override {
			array<bool, 2>^ result = gcnew array<bool, 2>(6, 6);
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 6; j++) {
					if (this->Move(x, y, i, j, cells)) {
						result[i, j] = true;
					}
					else {
						result[i, j] = false;
					}
				}
			}
			return result;
		}
	};
	void Figure::Draw(Graphics^ graphics, int x, int y, int length) 
	{
		graphics->DrawImage(Image::FromFile(path), x * length, y * length, length, length);
	}
	ref class Pawn : public Figure {
	public:
		Pawn() : Figure(){
			path = "figures\\Chess_tile_pl.png";
		}
		Pawn(bool colour): Figure(colour){
			if (colour) {
				path = "figures\\Chess_tile_pl.png";
			}
			else {
				path = "figures\\Chess_tile_pd.png";
			}
		}
		bool Move(int x, int y, int new_x, int new_y, array<int, 2>^ cells) override {
			if (x == new_x && y == new_y) {
				return false;
			}
			if (colour) {
				if (y - new_y > 1 || y - new_y < 0) {
					return false;
				}
			}
			else {
				if (new_y - y > 1 || new_y - y < 0) {
					return false;
				}
			}
			if (new_x != x) {
				if (abs(new_x - x) > 1) {
					return false;
				}
				if (cells[new_x, new_y] == -1) {
					return false;
				}
				if (Convert::ToBoolean(cells[new_x, new_y]) == colour) {
					return false;
				}
				return true;
			}
			if (cells[new_x, new_y] != -1) {
				return false;
			}
			return true;
		}
		
	};
	ref class King : public Figure {
	public:
		King() : Figure() {
			path = "figures\\Chess_tile_kl.png";
		}
		King(bool colour) : Figure(colour) {
			if (colour) {
				path = "figures\\Chess_tile_kl.png";
			}
			else {
				path = "figures\\Chess_tile_kd.png";
			}
		}
		virtual bool Move(int x, int y, int new_x, int new_y, array<int, 2>^ cells) override {
			if (x == new_x && y == new_y) {
				return false;
			}
			if (abs(x - new_x) > 1 || abs(y - new_y) > 1) {
				return false;
			}

			if (Convert::ToBoolean(cells[new_x, new_y]) == colour && cells[new_x, new_y] != -1) {
				return false;
			}
			return true;
		};
	};
	ref class Queen : public Figure {
	public:
		Queen() : Figure() {
			path = "figures\\Chess_tile_ql.png";
		}
		Queen(bool colour) : Figure(colour) {
			if (colour) {
				path = "figures\\Chess_tile_ql.png";
			}
			else {
				path = "figures\\Chess_tile_qd.png";
			}
		}
		bool HelperMove(int x, int y, int new_x, int new_y, int plus_x, int plus_y, array<int, 2>^ cells) {
			int i = x; int j = y;
			while (true) {
				i += plus_x;
				j += plus_y;
				if (i == new_x && j == new_y) {
					break;
				}
				if (cells[i, j] != -1) {
					return false;
				}
				
				
			}
			return true;
		}
		virtual bool Move(int x, int y, int new_x, int new_y, array<int, 2>^ cells) override{
			if (x == new_x && y == new_y) {
				return false;
			}
			if (Convert::ToBoolean(cells[new_x, new_y]) == colour && cells[new_x, new_y] != -1) {
				return false;
			}
			if ((x - new_x) != 0 && (y - new_y) != 0) {
				if (abs(x - new_x) != abs(y - new_y)) {
					return false;
				}
			}
			int plus_x = 0, plus_y = 0;
			if (x > new_x) {
				plus_x = -1;
			}
			else if (x < new_x) {
				plus_x = 1;
			}
			if (y > new_y) {
				plus_y = -1;
			}
			else if (y < new_y) {
				plus_y = 1;
			}
			return HelperMove(x, y, new_x, new_y, plus_x, plus_y, cells);
		};
	};
	ref class Knight : public Figure {
	public:
		Knight() : Figure() {
			path = "figures\\Chess_tile_nl.png";
		}
		Knight(bool colour) : Figure(colour) {
			if (colour) {
				path = "figures\\Chess_tile_nl.png";
			}
			else {
				path = "figures\\Chess_tile_nd.png";
			}
		}
		virtual bool Move(int x, int y, int new_x, int new_y, array<int, 2>^ cells) override {
			if (x == new_x && y == new_y) {
				return false;
			}
			if (abs(x - new_x) + abs(y - new_y) != 3) {
				return false;
			}
			if (abs(x - new_x) > 2 ||  abs(y - new_y) > 2) {
				return false;
			}
			if (abs(x - new_x) == 2 && abs(y - new_y) != 1) {
				return false;
			}
			if (abs(x - new_x) == 1 && abs(y - new_y) != 2) {
				return false;
			}
			if (Convert::ToBoolean(cells[new_x, new_y]) == colour && cells[new_x, new_y] != -1) {
				return false;
			}
			return true;
		};
	};
	ref class Rook : public Figure {
	private:
		bool HelperMove(int x, int y, int new_x, int new_y, int plus_x, int plus_y, array<int, 2>^ cells) {
			for (int i = x + plus_x, j = y + plus_y; i != new_x || j != new_y; i += plus_x, j += plus_y) {
				if (cells[i, j] != -1) {
					return false;
				}
			}
			return true;
		}
	public:
		Rook() : Figure() {
			path = "figures\\Chess_tile_rl.png";
		}
		Rook(bool colour) : Figure(colour) {
			if (colour) {
				path = "figures\\Chess_tile_rl.png";
			}
			else {
				path = "figures\\Chess_tile_rd.png";
			}
		}
		
		virtual bool Move(int x, int y, int new_x, int new_y, array<int, 2>^ cells) override {
			if (x == new_x && y == new_y) {
				return false;
			}
			if (Convert::ToBoolean(cells[new_x, new_y]) == colour && cells[new_x, new_y] != -1) {
				return false;
			}
			if (abs(x - new_x) > 0 && abs(y - new_y) > 0) {
				return false;
			}
			int plus_x = 0, plus_y = 0;
			if (x > new_x) {
				plus_x = -1;
			}
			else if (x < new_x) {
				plus_x = 1;
			}
			if (y > new_y) {
				plus_y = -1;
			}
			else if (y < new_y) {
				plus_y = 1;
			}
			return HelperMove(x, y, new_x, new_y, plus_x, plus_y, cells);
		};
	};
};