#pragma once

#include "json.hpp"

#include "Vector3.h"
#include "Vector2.h"
#include "Size2D.h"

#include <vector>
#include "SDL\include\SDL.h"

using namespace std;

using nlohmann::json;

void to_json(json& j, const Vector3& v) {
	j = json{ {"x",v.x}, {"y",v.y}, {"z",v.z} };
}

void from_json(const json& j, Vector3& v) {
	v.x = j.at("x").get<float>();
	v.y = j.at("y").get<float>();
	v.z = j.at("z").get<float>();
}

void to_json(json& j, const Vector2& v) {
	j = json{ { "x",v.x },{ "y",v.y } };
}

void from_json(const json& j, Vector2& v) {
	v.x = j.at("x").get<float>();
	v.y = j.at("y").get<float>();
}

void to_json(json& j, const Size2D& s) {
	j = json{ { "width", s.width },{ "height",s.height } };
}

void from_json(const json& j, Size2D& v) {
	v.width = j.at("width").get<float>();
	v.height = j.at("height").get<float>();
}

void to_json(json& j, const Animation& a) {
	j = json{ {"frames", a.frames}, {"loop", a.loop}, {"speed",a.speed} };
}

void from_json(const json& j, Animation& a) {
	a.frames = j.at("frames").get<vector<SDL_Rect>>();
	a.loop = j.at("loop").get<bool>();
	a.speed = j.at("speed").get<float>();
}

void to_json(json& j, const SDL_Rect& a) {
	j = json{a.x,a.y,a.w,a.h};
}

void from_json(const json& j, SDL_Rect& a) {
	a.x = j.at(0).get<int>();
	a.y = j.at(1).get<int>();
	a.w = j.at(2).get<int>();
	a.h = j.at(3).get<int>();
}