#include "Common.h"

using namespace std;

inline bool IsPointInRectangle(Point p, Size size) {
	return p.x < size.width && p.y < size.height;
}

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`

class Rectangle : public IShape {
public:
	//virtual ~IShape() = default;

	// Возвращает точную копию фигуры.
	// Если фигура содержит текстуру, то созданная копия содержит ту же самую
	// текстуру. Фигура и её копия совместно владеют этой текстурой.
	std::unique_ptr<IShape> Clone() const override {
		return unique_ptr<Rectangle>(new Rectangle(*this));
	}

	void SetPosition(Point p) override {
		point = p;
	}

	Point GetPosition() const override {
		return point;
	}

	void SetSize(Size s) override {
		size = s;
	}

	Size GetSize() const override {
		return size;
	}

	void SetTexture(std::shared_ptr<ITexture> tex) override {
		texture = tex;
	}

	ITexture* GetTexture() const override {
		return texture.get();
	}

	// Рисует фигуру на указанном изображении
	void Draw(Image& image) const override {
		if (image.empty() || image[0].empty())
			return;

		Size imageSize = { image[0].size(), image.size() };
		
		Point xy = point;

		for (; (xy.y < (size.height + point.y) && xy.y < imageSize.height); ++xy.y) {
			for (xy.x = point.x; (xy.x < (size.width + point.x) && xy.x < imageSize.width); ++xy.x) {
				// нормирование в начало координат
				Point shift_xy = { xy.x - point.x, xy.y - point.y };

				if (texture && IsPointInRectangle(shift_xy, texture->GetSize())) {
					const Image& tex = texture->GetImage();
					//if (tex[shift_xy.y][shift_xy.x] == ' ')
					//	image[xy.y][xy.x] = '.';
					//else
						image[xy.y][xy.x] = tex[shift_xy.y][shift_xy.x];
				}
				else {
					image[xy.y][xy.x] = '.';
				}
			}
		}
	}

private:
	Point point;
	Size size;
	shared_ptr<ITexture> texture;
};

class Ellipse : public IShape {
public:
	//virtual ~IShape() = default;

	// Возвращает точную копию фигуры.
	// Если фигура содержит текстуру, то созданная копия содержит ту же самую
	// текстуру. Фигура и её копия совместно владеют этой текстурой.
	std::unique_ptr<IShape> Clone() const override {
		return unique_ptr<Ellipse>(new Ellipse(*this));
	}

	void SetPosition(Point p) override {
		point = p;
	}

	Point GetPosition() const override {
		return point;
	}

	void SetSize(Size s) override {
		size = s;
	}

	Size GetSize() const override {
		return size;
	}

	void SetTexture(std::shared_ptr<ITexture> tex) override {
		texture = tex;
	}

	ITexture* GetTexture() const override {
		return texture.get();
	}

	// Рисует фигуру на указанном изображении
	void Draw(Image& image) const override {
		if (image.empty() || image[0].empty())
			return;

		Size imageSize = { image[0].size(), image.size() };

		Point xy = point;

		for (; (xy.y < (size.height + point.y) && xy.y < imageSize.height); ++xy.y) {
			for (xy.x = point.x; (xy.x < (size.width + point.x) && xy.x < imageSize.width); ++xy.x) {
				// нормирование в начало координат
				Point shift_xy = {xy.x - point.x, xy.y - point.y};

				if (IsPointInEllipse({ shift_xy.x, shift_xy.y }, size)) {
					if (texture && IsPointInRectangle(shift_xy, texture->GetSize())) {
						const Image& tex = texture->GetImage();
						//if (tex[shift_xy.y][shift_xy.x] == ' ')
						//	image[xy.y][xy.x] = '.';
						//else
							image[xy.y][xy.x] = tex[shift_xy.y][shift_xy.x];
					}
					else {
						image[xy.y][xy.x] = '.';
					}
				}
			}
		}
	}

private:
	Point point;
	Size size;
	shared_ptr<ITexture> texture;
};

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
	if(shape_type == ShapeType::Rectangle)
		return make_unique<Rectangle>();
	else
		return make_unique<Ellipse>();
}