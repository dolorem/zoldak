#pragma once

#include <QtCore>

#include <array>
#include <map>

#include "PlayerAction.h"
#include "InputAction.h"

namespace Zk {
namespace Game {

class InputConfig
{
public:
	InputConfig();
	
	friend QDataStream & operator<<(QDataStream & ds, const InputConfig & ic);
	friend QDataStream & operator>>(QDataStream & ds, InputConfig & ic);
	
	std::map<PlayerAction, InputAction> playerToInput;
};

//Oto wielka tajemnica: dlaczego dwie następne linijki mogę pominąć,
//a w przypadku GraphicsConfig muszę explicite pisać na zwenątrz??
// QDataStream & operator<<(QDataStream & ds, const InputConfig & ic);
// QDataStream & operator>>(QDataStream & ds, InputConfig & ic);

}}