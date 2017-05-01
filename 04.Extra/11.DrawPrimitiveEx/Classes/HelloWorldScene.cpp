#include "HelloWorldScene.h"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}


bool HelloWorld::init()
{

    if (!Layer::init())
    {
        return false;
    }

	this->newDraw();
    return true;
}

void HelloWorld::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	//_customCommand.init(_globalZOrder);
	//_customCommand.func = CC_CALLBACK_0(HelloWorld::onDraw, this, transform, flags);
	//renderer->addCommand(&_customCommand);
}

void HelloWorld::onDraw(const Mat4 &transform, uint32_t flags)
{
	Director* director = Director::getInstance();
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

	//draw
	CHECK_GL_ERROR_DEBUG();

	// draw a simple line
	// The default state is:
	// Line Width: 1
	// color: 255,255,255,255 (white, non-transparent)
	// Anti-Aliased
	//  glEnable(GL_LINE_SMOOTH);
	DrawPrimitives::drawLine(Vec2(0, 0), Vec2(480, 320));

	CHECK_GL_ERROR_DEBUG();

	// line: color, width, aliased
	// glLineWidth > 1 and GL_LINE_SMOOTH are not compatible
	// GL_SMOOTH_LINE_WIDTH_RANGE = (1,1) on iPhone
	//  glDisable(GL_LINE_SMOOTH);
	glLineWidth(5.0f);
	DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
	DrawPrimitives::drawLine(Vec2(0, 320), Vec2(480, 0));

	CHECK_GL_ERROR_DEBUG();

	// TIP:
	// If you are going to use always thde same color or width, you don't
	// need to call it before every draw
	//
	// Remember: OpenGL is a state-machine.

	// draw big point in the center
	DrawPrimitives::setPointSize(64);
	DrawPrimitives::setDrawColor4B(0, 0, 255, 128);
	DrawPrimitives::drawPoint(Vec2(240, 160));

	CHECK_GL_ERROR_DEBUG();

	// draw 4 small points
	Vec2 points[] = { Vec2(60,60), Vec2(70,70), Vec2(60,70), Vec2(70,60) };
	DrawPrimitives::setPointSize(4);
	DrawPrimitives::setDrawColor4B(0, 255, 255, 255);
	DrawPrimitives::drawPoints(points, 4);

	CHECK_GL_ERROR_DEBUG();

	// draw a green circle with 10 segments
	glLineWidth(16);
	DrawPrimitives::setDrawColor4B(0, 255, 0, 255);
	DrawPrimitives::drawCircle(Vec2(240, 160), 100, 0, 10, false);

	CHECK_GL_ERROR_DEBUG();

	// draw a green circle with 50 segments with line to center
	glLineWidth(2);
	DrawPrimitives::setDrawColor4B(0, 255, 255, 255);
	DrawPrimitives::drawCircle(Vec2(240, 160), 50, CC_DEGREES_TO_RADIANS(90), 50, true);

	CHECK_GL_ERROR_DEBUG();

	// draw a pink solid circle with 50 segments
	glLineWidth(2);
	DrawPrimitives::setDrawColor4B(255, 0, 255, 255);
	DrawPrimitives::drawSolidCircle(Vec2(240 ,160) + Vec2(140, 0), 40, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f);

	CHECK_GL_ERROR_DEBUG();

	// open yellow poly
	DrawPrimitives::setDrawColor4B(255, 255, 0, 255);
	glLineWidth(10);
	Vec2 vertices[] = { Vec2(0,0), Vec2(50,50), Vec2(100,50), Vec2(100,100), Vec2(50,100) };
	DrawPrimitives::drawPoly(vertices, 5, false);

	CHECK_GL_ERROR_DEBUG();

	// filled poly
	glLineWidth(1);
	Vec2 filledVertices[] = { Vec2(0,120), Vec2(50,120), Vec2(50,170), Vec2(25,200), Vec2(0,170) };
	DrawPrimitives::drawSolidPoly(filledVertices, 5, Color4F(0.5f, 0.5f, 1, 1));


	// closed purble poly
	DrawPrimitives::setDrawColor4B(255, 0, 255, 255);
	glLineWidth(2);
	Vec2 vertices2[] = { Vec2(30,130), Vec2(30,230), Vec2(50,200) };
	DrawPrimitives::drawPoly(vertices2, 3, true);

	CHECK_GL_ERROR_DEBUG();

	// draw quad bezier path
	DrawPrimitives::drawQuadBezier(Vec2(480, 0), Vec2(240, 160), Vec2(480, 320), 50);

	CHECK_GL_ERROR_DEBUG();

	// draw cubic bezier path
	DrawPrimitives::drawCubicBezier(Vec2(240, 160), Vec2(Vec2(240, 160).x + 30, Vec2(240, 160).y + 50), Vec2(Vec2(240, 160).x + 60, Vec2(240, 160).y - 50), Vec2(240, 160), 100);

	CHECK_GL_ERROR_DEBUG();

	//draw a solid polygon
	Vec2 vertices3[] = { Vec2(60,160), Vec2(70,190), Vec2(100,190), Vec2(90,160) };
	DrawPrimitives::drawSolidPoly(vertices3, 4, Color4F(1, 1, 0, 1));

	// restore original values
	glLineWidth(1);
	DrawPrimitives::setDrawColor4B(255, 255, 255, 255);
	DrawPrimitives::setPointSize(1);

	CHECK_GL_ERROR_DEBUG();

	//end draw
	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void HelloWorld::newDraw()
{
	auto draw_node = DrawNode::create();
	this->addChild(draw_node);

	// draw line
	draw_node->drawLine(Vec2(0, 0), Vec2(480, 320), Color4F(1, 1, 1, 1));

	// draw line
	draw_node->drawLine(Vec2(0, 320), Vec2(480, 0), Color4F(1, 0, 0, 1));
//  draw_node->drawSegment(Vec2(0, 320), Vec2(480, 0), 5 , Color4F(1, 0, 0, 1))

	// draw big point in the center
	draw_node->drawPoint(Vec2(240, 160), 64.0f, Color4F(0, 0, 1, 0.5));

	// draw 4 small points
	Vec2 points[] = { Vec2(60, 60), Vec2(70, 70), Vec2(60, 70), Vec2(70, 60) };
	draw_node->drawPoints(points, 4, Color4F(0, 1, 1, 1));

	// draw a green circle with 10 segments
	draw_node->drawCircle(Vec2(240, 160), 100, CC_DEGREES_TO_RADIANS(0), 10, false, Color4F(0, 1, 0, 1));

	// draw a green circle with 50 segments with line to center
	draw_node->drawCircle(Vec2(240, 160), 50, CC_DEGREES_TO_RADIANS(90), 50, true, Color4F(0, 1, 1, 1));

	// draw a pink solid circle with 50 segments
	draw_node->drawSolidCircle(Vec2(240, 160) + Vec2(140, 0), 40, 
		CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, Color4F(1, 0, 1, 1));

	// open yellow poly
	Vec2 vertices[] = { Vec2(0, 0), Vec2(50, 50), Vec2(100, 50), Vec2(100, 100), Vec2(50, 100) };
	draw_node->drawPoly(vertices, 5, false, Color4F(1, 1, 0, 1));

	// filled poly
	Vec2 filledVertices[] = { Vec2(0, 120), Vec2(50, 120), Vec2(50, 170), Vec2(25, 200), Vec2(0, 170) };
	draw_node->drawSolidPoly(filledVertices, 5, Color4F(0.5f, 0.5f, 1, 1));

	// closed purble poly
	Vec2 vertices2[] = { Vec2(30, 130), Vec2(30, 230), Vec2(50, 200) };
	draw_node->drawPoly(vertices2, 3, true, Color4F(1, 0, 1, 1));
	
	// draw quad bezier path
	draw_node->drawQuadBezier(Vec2(0, 320), Vec2(240, 160), Vec2(480, 320), 50, Color4F(1, 0, 1, 1));
	// draw cubic bezier path
	draw_node->drawCubicBezier(
		Vec2(240, 160),
		Vec2(Vec2(240, 160).x + 30, Vec2(240, 160).y + 50),
		Vec2(Vec2(240, 160).x + 60, Vec2(240, 160).y - 50),
		Vec2(240, 160),
		100,
		Color4F(1, 0, 1, 1));
	
}