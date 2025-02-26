#include "line.hpp"

LineSegment::LineSegment() : LineSegment(0.0f, 0.0f, 0.0f, 0.0f) {}
LineSegment::LineSegment(float _x1, float _y1, float _x2, float _y2) :
	x1(_x1), y1(_y1), x2(_x2), y2(_y2) {};

std::string Point::toString() const{
	std::stringstream ss;
	ss << x << ' ' << y;
	return ss.str();
}

unsigned int LineDrawer::VBO;
unsigned int LineDrawer::VAO;
unsigned int LineDrawer::EBO;
bool LineDrawer::s_is_ready = false;
std::optional<ShaderLibrary::MVPShader> LineDrawer::m_line_shader = std::nullopt;

void LineDrawer::setup() {
	if (s_is_ready) return;

	float just_line[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	/*
	float line_data[] = {
		-0.1f, 0.0f,
		-0.1f, 1.0f,
		0.1f, 1.0f,
		0.1f, 0.0f,
	};
	*/

	unsigned int indices[] = {
		0, 1, 2,
		3, 4, 5
	};
	m_line_shader = ShaderLibrary::MVPShader();
	ShaderLibrary::MVPShader& shader = m_line_shader.value();
	Shader::FragmentShader colour = ShaderLibrary::generateFragmentShader("colour.frag");
	shader.addFragmentShader(colour);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(line_data), line_data, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(just_line), just_line, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	s_is_ready = true;
}

void LineDrawer::draw(float x1, float y1, float x2, float y2, const glm::mat4& mvp) {
	if (!s_is_ready) {
		setup();
	}

	float line_data[] = {
		x1, y1, 0.0f,
		x2, y2, 0.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_data), line_data, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);	// Vertex attributes stay the same
	//glEnableVertexAttribArray(0);

	ShaderLibrary::MVPShader& shader = m_line_shader.value();
	shader.use();
	shader.setMVP(mvp);
	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_LINES, 0, 2);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
}

void LineDrawer::drawLineSegment(const LineSegment& ls, const glm::mat4& mvp) {
	draw(ls.x1, ls.y1, ls.x2, ls.y2, mvp);
}

void LineDrawer::setColour(const Colour& c) {
	if (!s_is_ready) {
		setup();
	}
	ShaderLibrary::MVPShader& shader = m_line_shader.value();
	shader.use();
	shader.setVec3("colour", c.toVec3());
}

void LineDrawer::clean(){
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	s_is_ready = false;
}

void LineCircleDrawer::draw(float x, float y, float radius, const glm::mat4& mvp, unsigned int n_points) {
	float angle = 0.0f;
	float angle_increment = 360.0f / static_cast<float>(n_points);
	std::vector<std::pair<float, float>> points;
	for (unsigned int i = 0; i < n_points; i++) {
		float c = cos(glm::radians(angle)) * radius;
		float s = sin(glm::radians(angle)) * radius;
		angle += angle_increment;
		points.push_back(std::make_pair(c + x, s + y));
	}
	for (size_t i = 1; i < points.size(); ++i) {
		LineDrawer::draw(points[i - 1].first, points[i - 1].second, points[i].first, points[i].second, mvp);
	}
	size_t last = points.size() - 1;
	LineDrawer::draw(points[0].first, points[0].second, points[last].first, points[last].second, mvp);
}



void LineCircleDrawer::setColour(const Colour& c) {
	LineDrawer::setColour(c);
}

std::string lineCircleCollisionTypeToString(LineCircleCollisionType type) {
	switch (type) {
	case LineCircleCollisionType::NoCollision:
		return "No Collision";
	case LineCircleCollisionType::Intersecting:
		return "Intersecting";
	case LineCircleCollisionType::Touching:
		return "Touching";
	}
}

std::string lineRectCollisionTypeToString(LineRectCollisionType type){
	switch (type) {
	case LineRectCollisionType::NoCollision:
		return "No Collision";
	case LineRectCollisionType::TouchingCorner:
		return "Touching Corner";
	case LineRectCollisionType::Intersecting:
		return "Intersecting";
	case LineRectCollisionType::OnHorizontal:
		return "On Horizontal";
	case LineRectCollisionType::OnVertical:
		return "On Vertical";
	}
}

CollisionLine::CollisionLine(): CollisionLine(0.0f, 0.0f, Vector2D(1.0f, 0.0f)) {};

CollisionLine::CollisionLine(float x, float y, const Vector2D& dir, bool is_segment)
	: m_x1(x), m_y1(y), m_x2(x + dir.getX()), m_y2(y + dir.getY()), m_direction(dir.normal()),
	m_segment(is_segment), m_a(dir.getY()), m_b(-dir.getX()), m_c(y*dir.getX()-x*dir.getY())
{

}

CollisionLine CollisionLine::from2Points(float x1, float y1, float x2, float y2, bool is_segment) {
	Vector2D v(x2 - x1, y2 - y1);
	return CollisionLine(x1, y1, v);
}

CollisionLine CollisionLine::fromLineSegment(const LineSegment& ls, bool is_segment) {
	return from2Points(ls.x1, ls.y1, ls.x2, ls.y2, is_segment);
}

LineSide CollisionLine::sideOfPoint(float x, float y) const {
	float value = pointEquationValue(x, y);
	if (value > 0) {
		return LineSide::Right;
	}
	else if (value < 0) {
		return LineSide::Left;
	}
	return LineSide::OnPoint;
}

Point CollisionLine::getStart() const {
	return Point(m_x1, m_y1);
}

Point CollisionLine::getEnd() const {
	return Point(m_x2, m_y2);
}

const Vector2D& CollisionLine::getDirection() const {
	return m_direction;
}

float CollisionLine::getX(float y) const{
	return -(m_b*y + m_c)/m_a;
}

float CollisionLine::getY(float x) const {
	return -(m_a * x + m_c)/m_b;
}


float CollisionLine::pointEquationValue(float x, float y) const {
	return (m_a * x) + (m_b * y) + m_c;
}

float CollisionLine::closestDistanceToPoint(float px, float py) const {
	float dx = m_x2 - m_x1;
	float dy = m_y2 - m_y1;
	float numerator = dy * px - dx * py + m_x2*m_y1 - m_y2*m_x1;
	Vector2D perpendicular = m_direction.perpendicular();
	return numerator;
}

bool CollisionLine::isPointOnLine(float px, float py, float error) const {
	//todo
	float dx = m_x2 - m_x1;
	float dy = m_y2 - m_y1;
	//float distance = 
	if (!m_segment) {
		//m_x1
	}
	return true;
}

bool CollisionLine::isVertical() const {
	return m_a != 0 && m_b == 0;
}

bool CollisionLine::isHorizontal() const {
	return m_b != 0 && m_a == 0;
}
bool CollisionLine::isLine() const{
	return m_b != 0 && m_a != 0;
}

LineGradientInterceptForm CollisionLine::getGradientInterceptForm() const {
	float dx = m_x2 - m_x1;
	float dy = m_y2 - m_y1;
	if (m_b == 0) {
		if (m_a == 0) {
			return LineGradientInterceptForm(GradientInterceptType::NotLine);
		}
		return LineGradientInterceptForm(GradientInterceptType::Vertical, -m_c / m_a);
	}
	return LineGradientInterceptForm(GradientInterceptType::GradientIntercept, -m_a / m_b, -m_c / m_b);
}

const LineCircleCollisionDetails CollisionLine::collisionLineToCircleDetails(const CollisionLine& line, const CollisionCircle& cc) {
	LineGradientInterceptForm line_form = line.getGradientInterceptForm();
	float radius = cc.getRadius();
	if (line_form.type == GradientInterceptType::GradientIntercept) {

		QuadraticCoefficients coeffXPart = getLinearPolynomialSquareCoefficients(1.0f, -cc.getX()); // (x-h)^2
		QuadraticCoefficients coeffYPart = getLinearPolynomialSquareCoefficients(line_form.m, line_form.b - cc.getY()); // (y-k)^2 -> [sub y] (mx+b-h)^2
		float a = coeffXPart.a + coeffYPart.a;
		float b = coeffXPart.b + coeffYPart.b;
		float c = coeffXPart.c + coeffYPart.c - (radius*radius);
		QuadraticSolution solution_x = solveQuadratic(a, b, c);
		if (solution_x.type == QuadraticSolutionType::TwoRoots) {
			float y1 = line.getY(solution_x.x1);

			float y2 = line.getY(solution_x.x2);
			Point p1(solution_x.x1, y1);
			Point p2(solution_x.x2, y2);
			return LineCircleCollisionDetails(LineCircleCollisionType::Intersecting, p1, p2);
		}
		else if (solution_x.type == QuadraticSolutionType::OneRoot) {
			float y1 = line.getY(solution_x.x1);
			Point p1(solution_x.x1, y1);
			return LineCircleCollisionDetails(LineCircleCollisionType::Touching, p1);
		}
		else {
			return LineCircleCollisionDetails(LineCircleCollisionType::NoCollision);
		}
	}
	else if (line_form.type == GradientInterceptType::Vertical) {
		float sub = line_form.m - cc.getX();
		float sq_root = radius * radius - sub * sub;
		if (sq_root > 0) {
			//two solutions
			float root = std::sqrt(sq_root);
			float k = cc.getY();
			float y1 = k + root;
			float y2 = k - root;
			Point p1(line_form.m, y1);
			Point p2(line_form.m, y2);
			return LineCircleCollisionDetails(LineCircleCollisionType::Intersecting, p1, p2);
		}
		else if (sq_root < 0) {
			//no solutions
			return LineCircleCollisionDetails(LineCircleCollisionType::NoCollision);
		}
		else {
			//one solutions
			Point p1(line_form.m, cc.getY());
			return LineCircleCollisionDetails(LineCircleCollisionType::Touching, p1);
		}
	}

	//not line
	return LineCircleCollisionDetails();
}

const LineRectCollisionDetails CollisionLine::collisionLineToRectDetails(const CollisionLine& line, const CollisionRect& rect) {
	std::cout << std::to_string(line.m_a) << " " << std::to_string(line.m_b) << " " << std::to_string(line.m_c);
	if (line.isVertical()) {
		float x = line.getX(0);
		float left = rect.getLeft();
		float right = rect.getRight();
		if (x > left && x < right) {
			return LineRectCollisionDetails(LineRectCollisionType::Intersecting, Point(x, rect.getTop()), Point(x, rect.getBottom()));
		}
		if (x == left || x == right) {
			return LineRectCollisionDetails(LineRectCollisionType::OnVertical, Point(x, rect.getTop()), Point(x, rect.getBottom()));
		}
	}
	else if (line.isHorizontal()) {
		float y = line.getY(0);
		float top = rect.getTop();
		float bottom = rect.getBottom();
		if (y > top && y < bottom) {
			return LineRectCollisionDetails(LineRectCollisionType::Intersecting, Point(rect.getLeft(), y), Point(rect.getRight(), y));
		}
		if (y == top || y == bottom) {
			return LineRectCollisionDetails(LineRectCollisionType::OnHorizontal, Point(rect.getLeft(), y), Point(rect.getRight(), y));
		}
	}
	else if (line.isLine()) {
		float left = rect.getLeft(); float right = rect.getRight();
		float top = rect.getTop(); float bottom = rect.getBottom();
		float x_top = line.getX(top);
		float x_bottom = line.getX(bottom);
		float y_left = line.getY(left);
		float y_right = line.getY(right);
		std::vector<Point> points;
		if (rect.isInsideX(x_top)) {
			points.push_back(Point(x_top, top));
		}
		if (rect.isInsideX(x_bottom)) {
			points.push_back(Point(x_bottom, bottom));
		}
		if (rect.isInsideY(y_left) && !(y_left == top || y_left == bottom)) {
			points.push_back(Point(left, y_left));
		}
		if (rect.isInsideY(y_right) && !(y_right == top || y_right == bottom)) {
			points.push_back(Point(right, y_right));
		}
		if (points.size() == 2) {
			return LineRectCollisionDetails(LineRectCollisionType::Intersecting, points[0], points[1]);
		}
		if (points.size() == 1) {
			return LineRectCollisionDetails(LineRectCollisionType::TouchingCorner, points[0]);
		}
	}
	return LineRectCollisionDetails(LineRectCollisionType::NoCollision);
}
