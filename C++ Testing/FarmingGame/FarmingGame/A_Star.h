#ifndef __A_Star
#define __A_Star

#include <stack>
#include <math.h>

const int Width = 30;
const int Height = 30;

bool _KingMode = true;

#include <SFML/Graphics.hpp>

typedef struct Point {
	int x;
	int y;
} Point;

/*class Node {
	
};*/

typedef struct _Node {
	Point p;
	_Node * parent;
	double f;
	double g;
	double h;
} Node;

class SortedStack {
private:
	std::stack<Node> * vals;
public:
	SortedStack() {
		vals = new std::stack<Node>();
	}

	void push(Node value) {
		std::stack<Node> tmp;
		bool inserted = false;
		while (!vals->empty() && !inserted) {
			if (value.f < vals->top().f) {
				vals->push(value);
				inserted = true;
			}
			else {
				tmp.push(vals->top());
				vals->pop();
			}
		}
		if (!inserted) {
			vals->push(value);
		}
		while (!tmp.empty()) {
			vals->push(tmp.top());
			tmp.pop();
		}
	}
	Node pop() {
		Node val = vals->top();
		vals->pop();
		return val;
	}
	Node top() {
		return vals->top();
	}
	bool empty() {
		return vals->empty();
	}
	int size() {
		return vals->size();
	}

};

Point generateNodePoint(int successorIndex, Point * _orig) {
	Point orig;		orig.x = _orig->x;	orig.y = _orig->y;
	if (_KingMode) {
		if (successorIndex >= 4) {
			successorIndex++;
		}

		orig.x = orig.x + ((successorIndex % 3 == 0) ? -1 : ((successorIndex) % 3 == 2) ? 1 : 0);
		orig.y = orig.y + ((successorIndex / 3 == 0) ? -1 : ((successorIndex + 1) / 3 == 2) ? 1 : 0);

		return orig;
	}
	else {
		if (successorIndex == 0) { orig.y--; }
		if (successorIndex == 3) { orig.y++; }
		if (successorIndex == 1) { orig.x++; }
		if (successorIndex == 2) { orig.x--; }

		return orig;
	}
}

double calcDist(Point * a, Point * b) {
	double dx = a->x - b->x;	double dy = a->y - b->y;
	return sqrt(pow(dx, 2) + pow(dy, 2));
}

Node isPosInStack(std::stack<Node> * stk, Point * pos) {
	Node match; match.f = DBL_MAX;
	
	std::stack<Node> tmp;

	bool stop = false;
	while (!stk->empty() && !stop) {
		if (stk->top().p.x == pos->x && stk->top().p.y == pos->y) {
			match = stk->top();
			stop = true;
		}
		else {
			tmp.push(stk->top());
			stk->pop();
		}
	}
	while (!tmp.empty()) {
		stk->push(tmp.top());
		tmp.pop();
	}

	return match;
}
Node isPosInSortedStack(SortedStack * stk, Point * pos) {
	Node match; match.f = DBL_MAX;

	std::stack<Node> tmp;

	bool stop = false;
	while (!stk->empty() && !stop) {
		if (stk->top().p.x == pos->x && stk->top().p.y == pos->y) {
			match = stk->top();
			stop = true;
		}
		else {
			tmp.push(stk->top());
			stk->pop();
		}
	}
	while (!tmp.empty()) {
		stk->push(tmp.top());
		tmp.pop();
	}

	return match;
}

bool isNotValidPoint(Point * p, Point * blockedPoints, int blockedPointCount) {
	bool isBlocked = false;
	for (int i = 0; i < blockedPointCount; i++) {
		if (blockedPoints[i].x == p->x && blockedPoints[i].y == p->y) { isBlocked = true; }
	}

	return p->x < 0 || p->x >= Width || p->y < 0 || p->y >= Height || isBlocked;
}

void avoidDuplicates(std::stack<Node> * stk, Node * nVal) {
	std::stack<Node> tmp;
	bool stop = false;
	while (!stk->empty() && !stop) {
		if (stk->top().p.x == nVal->p.x && stk->top().p.y == nVal->p.y) {
			/// There is a duplicate!
			/// Avoid it!
			Node match = stk->top();
			if (match.f <= nVal->f) {
				/// Leave the stack as is
			}
			else {
				stk->pop();
				stk->push(*nVal);
			}
			stop = true;
		}
		else {
			tmp.push(stk->top());
			stk->pop();
		}
	}
	while (!tmp.empty()) {
		stk->push(tmp.top());
		tmp.pop();
	}
	if (!stop) {
		stk->push(*nVal);
	}
}

Node * a(Point * _start, Point * _dest, Point * blockedPoints, int blockedPointCount,Node*memNodes) {
	// The data for all the nodes so we only actually work with pointers to this memory block.
	int memNodesInd = 0;
	

	SortedStack * open = new SortedStack();
	std::stack<Node> closed;
	//Node * open_list = (Node *)calloc(Width * Height, sizeof(Node));
	//Node * closed_list = (Node *)calloc(Width * Height, sizeof(Node));
	
	Node start;			Node dest;
	start.p = *_start;	dest.p = *_dest;
	start.f = 0; start.g = 0; start.h = 0;
	//memNodes[_start.x + (_start.y * Width)] = start;

	open->push(start);


	while (!open->empty()) {
		
		/// Find lowest 'f', call it 'q'
		std::stack<Node> tmp;
		// Node with lowest 'f'
		//memNodes[memNodesInd] = open.top();
		//Node * q = &memNodes[memNodesInd++];
		Node q; q = open->top();
		while (!open->empty()) {
			if (q.f > open->top().f) {
				q = open->top();
			}
			tmp.push(open->top());
			open->pop();
		}
		while (!tmp.empty()) {
			open->push(tmp.top());
			tmp.pop();
		}

		while (!open->empty()) {
			if (q.f == open->top().f && q.p.x == open->top().p.x && q.p.y == open->top().p.y) {
			}
			else {
				tmp.push(open->top());
			}
			open->pop();
		}
		while (!tmp.empty()) {
			open->push(tmp.top());
			tmp.pop();
		}

		/// At this point we removed 'q' from 'open' (which is back to its original state otherwise).

		memNodesInd = q.p.x + (q.p.y * Width);
		memNodes[memNodesInd] = q;
		Node *qP = memNodes + memNodesInd;

		/// Generate the 'q''s next 8 successors
		int s1 = open->size();
		int s2 = closed.size();
		for (int i = 0; i < ((_KingMode) ? 8 : 4); i++) {
			Node successor;
			successor.p = generateNodePoint(i, &qP->p);
			successor.parent = qP;

			if (successor.p.x == _dest->x && successor.p.y == _dest->y) {
				/// The successor[i] is the destination. Stop searching!
				return qP;
			}
			else {
				successor.g = q.g + calcDist(&successor.p, &qP->p);
				successor.h = calcDist(&successor.p, _dest);
				successor.f = successor.g + successor.h;
			}

			Node matchInOpen = isPosInSortedStack(open, &successor.p);
			Node matchInClosed = isPosInStack(&closed, &successor.p);
			
			bool skipThisNode = (matchInOpen.f <= successor.f) || (matchInClosed.f <= successor.f) || isNotValidPoint(&successor.p, blockedPoints, blockedPointCount);

			if (!skipThisNode) {
				open->push(successor);
			}

		}
		
		avoidDuplicates(&closed, &q);
		//closed.push(q);

	}

	/*/// Find the node with the lowest 'f' in closed and return it
	Node best = closed.top();
	while (!closed.empty()) {
		if (closed.top().f < best.f && closed.top().f != 0) {
			best = closed.top();
		}
		closed.pop();
	}

	return &best;*/
	Node q = start;
	memNodesInd = q.p.x + (q.p.y * Width);
	memNodes[memNodesInd] = q;
	Node *qP = memNodes + memNodesInd;
	dest.parent = qP;
	return &dest;
}

Node * a_find_next(Point * _start, Point * _dest, Point * blockedPoints, int blockedPointCount, Node*memNodes) {
	Node * next;

	next = a(_start, _dest, blockedPoints, blockedPointCount,memNodes);

	return next;

	int recC = 0;
	while (calcDist(_start, &next->p) >= 2) {
		printf("Rec #%d\n", recC++);
		Point nextPoint = next->p;
		next = a(_start, &nextPoint, blockedPoints, blockedPointCount,memNodes);
	}

	return next;
}

int A_Star_Demo() {
	sf::RenderWindow window(sf::VideoMode(Width * 25, Height * 25), "A*");

	Point pS;	pS.x = 2;	pS.y = 2;
	Point pD;	pD.x = 8;	pD.y = 4;

	Point pT = pS;

	sf::Texture tBlank;		tBlank.loadFromFile("graphics/A_Star/blank.png");
	sf::Texture tPath;		tPath.loadFromFile("graphics/A_Star/path.png");
	sf::Texture tStart;		tStart.loadFromFile("graphics/A_Star/start.png");
	sf::Texture tDest;		tDest.loadFromFile("graphics/A_Star/dest.png");
	sf::Texture tBlocked;		tBlocked.loadFromFile("graphics/A_Star/blocked.png");
	

	char * board = (char *)calloc(Width * Height, sizeof(char)); // 0 = Blank, 1 = Path, 2 = Start, 3 = Dest, 4 = Blocked

	board[pS.x + pS.y * Width] = 2;
	board[pD.x + pD.y * Width] = 3;

	bool atTarget = false; bool pauseSearch = false;

	Point blockedPoints[Width * Height];
	int blockedPointsInd = 0;

	while (window.isOpen()) {

		// Move Destination
		sf::Event e; // SFML Window event
		while (window.pollEvent(e)) { // If an event needs to happen
			if (e.type == sf::Event::MouseButtonPressed) {
				for (int i = 0; i < Width * Height; i++) {
					if (board[i] != 4) {
						board[i] = 0;
					}
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					pD.x = sf::Mouse::getPosition(window).x / 25;
					pD.y = sf::Mouse::getPosition(window).y / 25;
				}
				else {
					Point toBlock;
					toBlock.x = sf::Mouse::getPosition(window).x / 25;
					toBlock.y = sf::Mouse::getPosition(window).y / 25;
					blockedPoints[blockedPointsInd++] = toBlock;
					board[toBlock.x + toBlock.y * Width] = (board[toBlock.x + toBlock.y * Width]==4) ? 0 : 4;
				}
				pT = pS;
				atTarget = false;
			}
			else if (e.type == sf::Event::KeyPressed) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
					pauseSearch = !pauseSearch;
				} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
					for (int i = 0; i < Width * Height; i++) {
						if (board[i] != 4) {
							board[i] = 0;
						}
					}
					_KingMode = !_KingMode;
					pT = pS;
					atTarget = false;
				}
			}
		}

		board[pS.x + pS.y * Width] = 2;
		board[pD.x + pD.y * Width] = 3;

		window.clear(); // Remove all window graphics

		for (int i = 0; i < Width * Height; i++) {
			int x = (i % Width) * 25;	int y = (i / Width) * 25;

			sf::Sprite block;
			switch (board[i])
			{
			case 0: { block.setTexture(tBlank);		break; }
			case 1: { block.setTexture(tPath);		break; }
			case 2: { block.setTexture(tStart);		break; }
			case 3: { block.setTexture(tDest);		break; }
			case 4: { block.setTexture(tBlocked);	break; }
			}

			block.setPosition(x, y);

			window.draw(block);
		}

		if (calcDist(&pT, &pD) < 2) { atTarget = true; }
		if (!atTarget && !pauseSearch) {
			printf("Starting search!\n");
			Node * memNodes = (Node *)malloc(Width * Height * sizeof(Node));
			Node *pat = a_find_next(&pT, &pD, blockedPoints, blockedPointsInd, memNodes);
			pT = pat->p;
			while (calcDist(&pat->p, &pS) >= 1) {
				board[pat->p.x + pat->p.y * Width] = 1;
				pat = pat->parent;
				window.display();
			}			
			free(memNodes);
		}


		window.display();

	}


	free(board);
	return 0;
}

#endif // !__A_Star