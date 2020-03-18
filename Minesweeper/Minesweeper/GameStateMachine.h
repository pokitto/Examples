#pragma once

//
// Copyright (C) 2018 Pharap (@Pharap)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

template< typename ContextType, typename StateIdType >
class GameState;

template< typename ContextType, typename StateIdType >
class GameStateMachine
{
public:
	using Context = ContextType;
	using StateId = StateIdType;
	using State = GameState<Context, StateId>;

public:
	virtual ~GameStateMachine(void) {}

	virtual Context & getContext(void) = 0;
	virtual const Context & getContext(void) const = 0;

	virtual void changeState(const StateId & stateId) = 0;
};