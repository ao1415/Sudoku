#include <Siv3D.hpp>

class AI {
private:

	Grid<int> table;

	bool checkLine(Grid<Array<int>>& answer) {

		bool flag = false;
		for (const auto& pos : step(answer.size()))
		{
			if ((int)answer[pos].size() > 1)
			{
				Array<bool> flags(10, false);
				for (int x = 0; x < (int)answer.width; x++)
				{
					flags[table[pos.y][x]] = true;
				}

				for (auto it = answer[pos].begin(); it != answer[pos].end();)
				{
					if (flags[*it])
					{
						it = answer[pos].erase(it);
						flag = true;
					}
					else
					{
						++it;
					}
				}
			}
		}

		return flag;
	}
	bool checkColumn(Grid<Array<int>>& answer) {

		bool flag = false;
		for (const auto& pos : step(answer.size()))
		{
			if ((int)answer[pos].size() > 1)
			{
				Array<bool> flags(10, false);
				for (int y = 0; y < (int)answer.height; y++)
				{
					flags[table[y][pos.x]] = true;
				}

				for (auto it = answer[pos].begin(); it != answer[pos].end();)
				{
					if (flags[*it])
					{
						it = answer[pos].erase(it);
						flag = true;
					}
					else
					{
						++it;
					}
				}
			}
		}

		return flag;
	}
	bool checkBlock(Grid<Array<int>>& answer) {

		bool flag = false;
		for (const auto& pos : step(answer.size()))
		{
			if ((int)answer[pos].size() > 1)
			{
				Array<bool> flags(10, false);

				for (int y = pos.y / 3 * 3; y < pos.y / 3 * 3 + 3; y++)
				{
					for (int x = pos.x / 3 * 3; x < pos.x / 3 * 3 + 3; x++)
					{
						flags[table[y][x]] = true;
					}
				}

				for (auto it = answer[pos].begin(); it != answer[pos].end();)
				{
					if (flags[*it])
					{
						it = answer[pos].erase(it);
						flag = true;
					}
					else
					{
						++it;
					}
				}
			}
		}

		return flag;
	}

	void check(Grid<Array<int>>& answer) {

		bool flag = true;

		while (flag)
		{
			flag = false;
			flag |= checkLine(answer);
			confirm(answer);
			flag |= checkColumn(answer);
			confirm(answer);
			flag |= checkBlock(answer);
			confirm(answer);
		}

	}

	void confirmLine(Grid<Array<int>>& answer) {

		for (const auto& pos : step(answer.size()))
		{
			if ((int)answer[pos].size() > 1)
			{
				Array<int> flags(10, 0);
				for (int x = 0; x < (int)answer.width; x++)
				{
					for (const auto& candidate : answer[pos.y][x])
					{
						flags[candidate]++;
					}
				}


				for (const auto& c : answer[pos])
				{
					if (flags[c] == 1)
					{
						answer[pos] = { c };
						break;
					}
				}
			}
		}

	}
	void confirmColumn(Grid<Array<int>>& answer) {

		for (const auto& pos : step(answer.size()))
		{
			if ((int)answer[pos].size() > 1)
			{
				Array<int> flags(10, 0);
				for (int y = 0; y < (int)answer.height; y++)
				{
					for (const auto& candidate : answer[y][pos.x])
					{
						flags[candidate]++;
					}
				}


				for (const auto& c : answer[pos])
				{
					if (flags[c] == 1)
					{
						answer[pos] = { c };
						break;
					}
				}
			}
		}

	}
	void confirmBlock(Grid<Array<int>>& answer) {

		for (const auto& pos : step(answer.size()))
		{
			if ((int)answer[pos].size() > 1)
			{
				Array<int> flags(10, 0);
				for (int y = pos.y / 3 * 3; y < pos.y / 3 * 3 + 3; y++)
				{
					for (int x = pos.x / 3 * 3; x < pos.x / 3 * 3 + 3; x++)
					{
						for (const auto& candidate : answer[y][x])
						{
							flags[candidate]++;
						}
					}
				}

				for (const auto& c : answer[pos])
				{
					if (flags[c] == 1)
					{
						answer[pos] = { c };
						break;
					}
				}
			}
		}

	}

	void confirm(Grid<Array<int>>& answer) {

		for (const auto& pos : step(answer.size()))
		{
			if ((int)answer[pos].size() == 1)
			{
				table[pos] = answer[pos].front();
			}
		}

		confirmLine(answer);
		confirmColumn(answer);
		confirmBlock(answer);

	}

public:


	AI() {

	}

	Grid<Array<int>> think(const Grid<int>& question) {

		Grid<Array<int>> answer(9, 9);

		for (const auto& pos : step(question.size()))
		{
			if (question[pos] != 0)
			{
				answer[pos] = { question[pos] };
			}
			else
			{
				answer[pos] = { 1,2,3,4,5,6,7,8,9 };
			}
		}

		table = question;

		check(answer);

		return answer;
	}

};

void Main() {

	Font font(16);
	Font fontMin(4);

	Array<Rect> keypads(10);
	for (size_t i = 0; i < keypads.size(); i++)
	{
		keypads[i].pos = Point(0, 32) * (int)i;
		keypads[i].size = Point(32, 32);
	}

	Grid<Rect> inputTable(9, 9);
	for (int y = 0; y < (int)inputTable.height; y++)
	{
		for (int x = 0; x < (int)inputTable.width; x++)
		{
			inputTable[y][x].pos = Point(32 * x, 32 * y) + Point(48, 0);
			inputTable[y][x].size = Point(32, 32);
		}
	}

	Rect answerButton(48, 32 * 9 + 16, 64, 32);

	Grid<int> question(9, 9);
	Grid<Array<int>> answer(9, 9);

	int inputNumber = 0;

	while (System::Update())
	{
		for (int i = 0; i < (int)keypads.size(); i++)
		{
			if (keypads[i].leftClicked)
			{
				inputNumber = i;
			}
		}

		if (Input::Key1.clicked) inputNumber = 1;
		if (Input::Key2.clicked) inputNumber = 2;
		if (Input::Key3.clicked) inputNumber = 3;
		if (Input::Key4.clicked) inputNumber = 4;
		if (Input::Key5.clicked) inputNumber = 5;
		if (Input::Key6.clicked) inputNumber = 6;
		if (Input::Key7.clicked) inputNumber = 7;
		if (Input::Key8.clicked) inputNumber = 8;
		if (Input::Key9.clicked) inputNumber = 9;

		for (int y = 0; y < (int)inputTable.height; y++)
		{
			for (int x = 0; x < (int)inputTable.width; x++)
			{
				if (inputTable[y][x].leftClicked)
				{
					question[y][x] = inputNumber;
				}
				if (inputTable[y][x].rightClicked)
				{
					question[y][x] = 0;
				}
			}
		}

		if (answerButton.leftClicked)
		{
			AI ai;
			answer = ai.think(question);
		}

		for (int i = 0; i < (int)keypads.size(); i++)
		{
			if (i == inputNumber)
			{
				keypads[i].draw(Palette::Lightblue);
			}
			else
			{
				keypads[i].draw(Palette::White);
			}
			keypads[i].drawFrame(1, 0, Palette::Black);

			if (i != 0)
				font(i).drawCenter(keypads[i].center, Palette::Black);
		}

		for (int y = 0; y < (int)inputTable.height; y++)
		{
			for (int x = 0; x < (int)inputTable.width; x++)
			{
				inputTable[y][x].draw(Palette::White);
				inputTable[y][x].drawFrame(1, 0, Palette::Black);

				if (question[y][x] != 0)
					font(question[y][x]).drawCenter(inputTable[y][x].center, Palette::Black);
			}
		}

		Rect(inputTable[0][0].pos, inputTable[0][0].size * 9).drawFrame(2, 0, Palette::Black);
		for (int y = 0; y < (int)inputTable.height; y += 3)
		{
			for (int x = 0; x < (int)inputTable.width; x += 3)
			{
				Rect(inputTable[y][x].pos, inputTable[y][x].size * 3).drawFrame(2, 0, Palette::Black);
			}
		}

		for (int y = 0; y < (int)answer.height; y++)
		{
			for (int x = 0; x < (int)answer.width; x++)
			{
				Rect cell(Point(32 * x, 32 * y) + Point(64 + 32 * 9, 0), Point(32, 32));

				cell.draw(Palette::White);
				cell.drawFrame(1, 0, Palette::Black);

				if (question[y][x] == 0)
				{
					if (!answer[y][x].empty())
					{
						if ((int)answer[y][x].size() == 1)
						{
							font(answer[y][x].front()).drawCenter(cell.center, Palette::Red);
						}
						else
						{
							for (int i = 0; i < (int)answer[y][x].size(); i++)
							{
								fontMin(answer[y][x][i]).draw(cell.pos + Point(10 * (i % 3), 10 * (i / 3)) + Point(3, 3), Palette::Black);
							}
						}
					}
				}
				else
				{
					font(question[y][x]).drawCenter(cell.center, Palette::Black);
				}
			}
		}

		Rect(Point(64 + 32 * 9, 0), Point(32, 32) * 9).drawFrame(2, 0, Palette::Black);
		for (int y = 0; y < (int)inputTable.height; y += 3)
		{
			for (int x = 0; x < (int)inputTable.width; x += 3)
			{
				Rect(Point(32 * x, 32 * y) + Point(64 + 32 * 9, 0), Point(32, 32) * 3).drawFrame(2, 0, Palette::Black);
			}
		}

		answerButton.draw(Palette::Lightgrey);
		font(L"開始").drawCenter(answerButton.center, Palette::Black);

	}

}
