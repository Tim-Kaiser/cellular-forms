#pragma once

template<typename ValueT>
class Vec2 {
public:
	Vec2(ValueT x, ValueT y);
	ValueT m_x;
	ValueT m_y;
};

template<typename ValueT>
Vec2<ValueT>::Vec2(ValueT x, ValueT y) {
	m_x = x;
	m_y = y;
}