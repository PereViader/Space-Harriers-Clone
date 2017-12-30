#ifndef _JSON_SERIALIZER_H_
#define _JSON_SERIALIZER_H_

#include "json.hpp"
using nlohmann::json;

class Vector3;
void to_json(json& j, const Vector3& v);
void from_json(const json& j, Vector3& v);

class Vector2;
void to_json(json& j, const Vector2& v);
void from_json(const json& j, Vector2& v);

class Size2D;
void to_json(json& j, const Size2D& s);
void from_json(const json& j, Size2D& v);

class Animation;
void to_json(json& j, const Animation& a);
void from_json(const json& j, Animation& a);

struct SDL_Rect;
void to_json(json& j, const SDL_Rect& a);
void from_json(const json& j, SDL_Rect& a);

#endif // !_JSON_SERIALIZER_H_