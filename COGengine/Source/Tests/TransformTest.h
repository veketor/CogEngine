#pragma once

#include "ofMain.h"
#include "CogEngine.h"


#ifdef TESTING
#else

class DummyShape : public Shape {
public:
	int width;
	int height;

	DummyShape(int width, int height) : width(width), height(height) {

	}

	float GetWidth() {
		return width;
	}

	float GetHeight() {
		return height;
	}
};


#include "catch.hpp"
using namespace Cog;
#include "TransformMath.h"

	TEST_CASE("Transformation test", "[class]")
	{
		COGEngine.Init();
		COGEngine.environment->SetScreenSizeHard(ofVec2f(400, 400));
		auto math = TransformMath();
		

		SECTION("Topleft corner")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(0, 0),  // position
				0, 
				CalcType::ABS,	// pos type
				ofVec2f(0, 0),  // anchor
				ofVec2f(0, 0),	// size 
				CalcType::ABS,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			REQUIRE(((int)tr.absPos.x) == 0, ((int)tr.absPos.y) == 0);
			delete root;
			delete first;
		}

		SECTION("Center")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(0.5f, 0.5f),  // position
				0,
				CalcType::ABS_PER,	// pos type
				ofVec2f(0, 0),  // anchor
				ofVec2f(0, 0),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			REQUIRE(((int)tr.absPos.x) == 200, ((int)tr.absPos.y) == 200);
			delete root;
			delete first;
		}

		SECTION("Center centered")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(0.5f, 0.5f),  // position
				0,
				CalcType::ABS_PER,	// pos type
				ofVec2f(0.5f, 0.5f),  // anchor
				ofVec2f(0, 0),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			REQUIRE(((int)tr.absPos.x) == 100, ((int)tr.absPos.y) == 100);
			delete root;
			delete first;
		}

		SECTION("Scaled centered")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(0.5f, 0.5f),  // position
				0,
				CalcType::ABS_PER,	// pos type
				ofVec2f(0.5f, 0.5f),  // anchor
				ofVec2f(2, 2),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			REQUIRE(((int)tr.absPos.x) == 0, ((int)tr.absPos.y) == 0);
			delete root;
			delete first;
		}

		SECTION("Grid")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(100, 50),  // position
				0,
				CalcType::GRID,	// pos type
				ofVec2f(1.0f, 1.0f),  // anchor
				ofVec2f(2, 2),	// size 
				CalcType::LOC,	// scale type
				100, 50);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			REQUIRE(((int)tr.absPos.x) == 0, ((int)tr.absPos.y) == 0);
			delete root;
			delete first;
		}

		SECTION("Child topLeft")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* second = new Node("second");
			second->SetShape(spt<DummyShape>(new DummyShape(100, 100)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(0, 0),  // position
				0,
				CalcType::LOC,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(1, 1),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			first->SetTransform(tr);
			Trans tr2 = Trans(0, 0);
			math.CalcTransform(tr2, second, first,
				ofVec2f(0, 0),  // position
				0,
				CalcType::LOC,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(1, 1),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr2.CalcAbsTransform(tr);
			REQUIRE(((int)tr.absPos.x) == 0, ((int)tr.absPos.y) == 0, ((int)tr2.absPos.x) == 0, ((int)tr2.absPos.y) == 0);
			delete root;
			delete first;
			delete second;
		}

		SECTION("Child bottomRight absolute")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* second = new Node("second");
			second->SetShape(spt<DummyShape>(new DummyShape(100, 100)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(0, 0),  // position
				0,
				CalcType::LOC,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(1, 1),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			first->SetTransform(tr);
			Trans tr2 = Trans(0, 0);
			math.CalcTransform(tr2, second, first,
				ofVec2f(200, 200),  // position
				0,
				CalcType::ABS,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(1, 1),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr2.CalcAbsTransform(tr);
			REQUIRE(((int)tr2.absPos.x) == 200, ((int)tr2.absPos.y) == 200);
			delete root;
			delete first;
			delete second;
		}

		SECTION("Child bottomRight percentage")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* second = new Node("second");
			second->SetShape(spt<DummyShape>(new DummyShape(100, 100)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(0, 0),  // position
				0,
				CalcType::LOC,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(1, 1),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			first->SetTransform(tr);
			Trans tr2 = Trans(0, 0);
			math.CalcTransform(tr2, second, first,
				ofVec2f(1,1),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(1, 1),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr2.CalcAbsTransform(tr);
			REQUIRE(((int)tr2.absPos.x) == 200, ((int)tr2.absPos.y) == 200);
			delete root;
			delete first;
			delete second;
		}

		SECTION("Child center")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* second = new Node("second");
			second->SetShape(spt<DummyShape>(new DummyShape(100, 100)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(0, 0),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(2, 2),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			first->SetTransform(tr);
			Trans tr2 = Trans(0, 0);
			math.CalcTransform(tr2, second, first,
				ofVec2f(0.5f, 0.5f),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(1, 1),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr2.CalcAbsTransform(tr);
			REQUIRE(((int)tr2.absPos.x) == 200, ((int)tr2.absPos.y) == 200);
			delete root;
			delete first;
			delete second;
		}

		SECTION("Child center absper")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* second = new Node("second");
			second->SetShape(spt<DummyShape>(new DummyShape(100, 100)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(0, 0),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(2, 2),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			first->SetTransform(tr);
			Trans tr2 = Trans(0, 0);
			math.CalcTransform(tr2, second, first,
				ofVec2f(0.5f, 0.5f),  // position
				0,
				CalcType::ABS_PER,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(1, 1),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr2.CalcAbsTransform(tr);
			REQUIRE(((int)tr2.absPos.x) == 200, ((int)tr2.absPos.y) == 200);
			delete root;
			delete first;
			delete second;
		}

		SECTION("Child bottom-right local")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* second = new Node("second");
			second->SetShape(spt<DummyShape>(new DummyShape(100, 100)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(0, 0),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(2, 2),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			first->SetTransform(tr);
			Trans tr2 = Trans(0, 0);
			math.CalcTransform(tr2, second, first,
				ofVec2f(200, 200),  // position
				0,
				CalcType::LOC,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(1, 1),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr2.CalcAbsTransform(tr);
			REQUIRE(((int)tr2.absPos.x) == 400, ((int)tr2.absPos.y) == 400);
			delete root;
			delete first;
			delete second;
		}

		SECTION("Child bottom-right scaled local")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* second = new Node("second");
			second->SetShape(spt<DummyShape>(new DummyShape(100, 100)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(0, 0),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(2, 2),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			first->SetTransform(tr);
			Trans tr2 = Trans(0, 0);
			math.CalcTransform(tr2, second, first,
				ofVec2f(200, 200),  // position
				0,
				CalcType::LOC,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(2, 2),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr2.CalcAbsTransform(tr);
			REQUIRE(((int)tr2.absPos.x) == 400, ((int)tr2.absPos.y) == 400);
			delete root;
			delete first;
			delete second;
		}

		SECTION("Three topleft")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* second = new Node("second");
			second->SetShape(spt<DummyShape>(new DummyShape(100, 100)));
			Node* third = new Node("third");
			third->SetShape(spt<DummyShape>(new DummyShape(50, 50)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(0, 0),  // position
				0,
				CalcType::LOC,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(1, 1),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			first->SetTransform(tr);
			Trans tr2 = Trans(0, 0);
			math.CalcTransform(tr2, second, first,
				ofVec2f(0, 0),  // position
				0,
				CalcType::LOC,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(1, 1),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr2.CalcAbsTransform(tr);
			second->SetTransform(tr2);
			Trans tr3 = Trans(0, 0);
			math.CalcTransform(tr3, third, second,
				ofVec2f(0, 0),  // position
				0,
				CalcType::LOC,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(1, 1),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr3.CalcAbsTransform(tr2);
			REQUIRE(((int)tr3.absPos.x) == 0, ((int)tr3.absPos.y) == 0);
			delete root;
			delete first;
			delete second;
			delete third;
		}

		SECTION("Three right-bottom percentage")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* second = new Node("second");
			second->SetShape(spt<DummyShape>(new DummyShape(100, 100)));
			Node* third = new Node("third");
			third->SetShape(spt<DummyShape>(new DummyShape(50, 50)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(0, 0),  // position
				0,
				CalcType::LOC,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(1, 1),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			first->SetTransform(tr);
			Trans tr2 = Trans(0, 0);
			math.CalcTransform(tr2, second, first,
				ofVec2f(1, 0),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(2,2),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr2.CalcAbsTransform(tr);
			second->SetTransform(tr2);
			Trans tr3 = Trans(0, 0);
			math.CalcTransform(tr3, third, second,
				ofVec2f(0, 1),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(2,2),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr3.CalcAbsTransform(tr2);
			REQUIRE(((int)tr3.absPos.x) == 200, ((int)tr3.absPos.y) == 200);
			delete root;
			delete first;
			delete second;
			delete third;
		}

		SECTION("Three centered absper")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* second = new Node("second");
			second->SetShape(spt<DummyShape>(new DummyShape(100, 100)));
			Node* third = new Node("third");
			third->SetShape(spt<DummyShape>(new DummyShape(50, 50)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(0, 0),  // position
				0,
				CalcType::LOC,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(1, 1),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			first->SetTransform(tr);
			Trans tr2 = Trans(0, 0);
			math.CalcTransform(tr2, second, first,
				ofVec2f(1, 0),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(2, 2),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr2.CalcAbsTransform(tr);
			second->SetTransform(tr2);
			Trans tr3 = Trans(0, 0);
			math.CalcTransform(tr3, third, second,
				ofVec2f(0.5f, 0.5f),  // position
				0,
				CalcType::ABS_PER,	// pos type
				ofVec2f(0.5f, 0.5f),  // anchor
				ofVec2f(2, 2),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr3.CalcAbsTransform(tr2);
			REQUIRE(((int)tr3.absPos.x) == 100, ((int)tr3.absPos.y) == 100);
			delete root;
			delete first;
			delete second;
			delete third;
		}

		SECTION("Bottomright for each next per/per")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* second = new Node("second");
			second->SetShape(spt<DummyShape>(new DummyShape(100, 100)));
			Node* third = new Node("third");
			third->SetShape(spt<DummyShape>(new DummyShape(50, 50)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(0, 0),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(1, 1),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			first->SetTransform(tr);
			Trans tr2 = Trans(0, 0);
			math.CalcTransform(tr2, second, first,
				ofVec2f(1, 1),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(1, 1),	// size 
				CalcType::PER,	// scale type
				0, 0);			// grid size
			tr2.CalcAbsTransform(tr);
			second->SetTransform(tr2);
			Trans tr3 = Trans(0, 0);
			math.CalcTransform(tr3, third, second,
				ofVec2f(1, 1),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0, 0),  // anchor
				ofVec2f(1,1),	// size 
				CalcType::PER,	// scale type
				0, 0);			// grid size
			tr3.CalcAbsTransform(tr2);
			REQUIRE(((int)tr3.absPos.x) == 400, ((int)tr3.absPos.y) == 400);
			delete root;
			delete first;
			delete second;
			delete third;
		}

		SECTION("Bottomright for each next per/absper")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* second = new Node("second");
			second->SetShape(spt<DummyShape>(new DummyShape(100, 100)));
			Node* third = new Node("third");
			third->SetShape(spt<DummyShape>(new DummyShape(50, 50)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(0, 0),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(0.5f,0.5f),	// size 
				CalcType::ABS_PER,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			first->SetTransform(tr);
			Trans tr2 = Trans(0, 0);
			math.CalcTransform(tr2, second, first,
				ofVec2f(1, 1),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(0.5f, 0.5f),	// size 
				CalcType::ABS_PER,	// scale type
				0, 0);			// grid size
			tr2.CalcAbsTransform(tr);
			second->SetTransform(tr2);
			Trans tr3 = Trans(0, 0);
			math.CalcTransform(tr3, third, second,
				ofVec2f(1, 1),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0, 0),  // anchor
				ofVec2f(0.5f,0.5f),	// size 
				CalcType::ABS_PER,	// scale type
				0, 0);			// grid size
			tr3.CalcAbsTransform(tr2);
			REQUIRE(((int)tr3.absPos.x) == 400, ((int)tr3.absPos.y) == 400);
			delete root;
			delete first;
			delete second;
			delete third;
		}

		SECTION("Bottomright for each next per/abs")
		{
			Node* first = new Node("first");
			first->SetShape(spt<DummyShape>(new DummyShape(200, 200)));
			Node* second = new Node("second");
			second->SetShape(spt<DummyShape>(new DummyShape(100, 100)));
			Node* third = new Node("third");
			third->SetShape(spt<DummyShape>(new DummyShape(50, 50)));
			Node* root = new Node("root");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, first, root,
				ofVec2f(0, 0),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(1,1),	// size 
				CalcType::ABS,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(root->GetTransform());
			first->SetTransform(tr);
			Trans tr2 = Trans(0, 0);
			math.CalcTransform(tr2, second, first,
				ofVec2f(1, 1),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0.0f, 0.0f),  // anchor
				ofVec2f(2,2),	// size 
				CalcType::ABS,	// scale type
				0, 0);			// grid size
			tr2.CalcAbsTransform(tr);
			second->SetTransform(tr2);
			Trans tr3 = Trans(0, 0);
			math.CalcTransform(tr3, third, second,
				ofVec2f(1, 1),  // position
				0,
				CalcType::PER,	// pos type
				ofVec2f(0, 0),  // anchor
				ofVec2f(4,4),	// size 
				CalcType::ABS,	// scale type
				0, 0);			// grid size
			tr3.CalcAbsTransform(tr2);
			REQUIRE(((int)tr3.absPos.x) == 400, ((int)tr3.absPos.y) == 400);
			delete root;
			delete first;
			delete second;
			delete third;
		}
	}

#endif
	