/*-- Lobby --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, NOFF, NOAM]; } 
public func SectionName() { return "CrazyLadders"; } 
public func HideInMenu() { return true; } 
public func IsCMCMinigame() { return true; }

local container;

public func Initialize()
{ 
  container = CreateObject(TIM1);
  g_Gamemaster->Enter(container);
  
  SetPlayList("@CMC_Cell.ogg");
  Music("@CMC_Cell.ogg");
  
  var rnd = Random(10)+40;
  for(var i = 0; i < rnd; i++)
  {
  	var rnd_length = Random(6)+4;
  	var rnd_xpos = Random(LandscapeWidth()-20)+20;
  	rnd_xpos -= rnd_xpos % 20;
  	var ladr = CreateObject(LADR, rnd_xpos, Random(LandscapeHeight()), -1);
  	ladr->Set(rnd_length);
  	
  	if(Random(3))
  		LocalN("horizontal", ladr) = true;
  }
  
  for(var i = 0; i < rnd; i++)
  {
  	var rnd_length = Random(6)+4;
  	var rnd_xpos = Random(LandscapeWidth()-20)+20;
  	rnd_xpos -= rnd_xpos % 20;
  	var ladr = CreateObject(LADR, rnd_xpos, Random(LandscapeHeight()), -1);
  	ladr->Set(rnd_length);
  	LocalN("n_climbable", ladr) = true;
  	
  	var rClr = Random(60)+100;
  	ladr->ColorLadderTo(RGB(rClr, rClr, rClr));
  	
  	if(Random(2))
  		LocalN("horizontal", ladr) = true;
  }
  
  if(!Random(8))
  	ScheduleCall(this, "EpicCrankness", 3);
  
  return _inherited(...);
}

public func InitializePlayer(int iPlr)
{
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];
	
	var ladders = FindObjects(Find_ID(LADR));
	var ladder = ladders[Random(GetLength(ladders))];
	while(LocalN("n_climbable", ladder))
		ladder = ladders[Random(GetLength(ladders))];
	
	SetRespawnPosition(GetX(ladder), GetY(ladder)+10, iPlr);
	
	return _inherited(iPlr, ...);
}

public func OnClonkRelaunch(object pCrew, bool fInitPlr) 
{ 
	if(!fInitPlr)
	{
		pCrew->Enter(container);
		if(ObjectCount2(Find_ID(PCMK), Find_NoContainer())-1 <= 1)
		{
			var effect = DelayedCall("EvalRound", 1);
			SetDelayedCallObj(effect, this);
		}
	}
	else
		CreateContents(RSHL, pCrew);
	
	return true;
}

local crankness;

public func EpicCrankness()
{
	crankness = 1;
	SetName(Format("%s (Epic Crankness Mode)", GetName()));

	SetPlayList("@CMC_PartyEnd.ogg");
	Music("@CMC_PartyEnd.ogg");
	for(var i = 0; i < Random(2)+3; i++)
	{
		var background = C4Id(Format("BD%02d", Random(9)+1));
		CreateObject(background, LandscapeWidth()/2, LandscapeHeight()+GetDefHeight(background)/3, -1);
 	}
 	
 	for(var background in FindObjects(Find_Func("IsDeco")))
 		AddEffect("CrankBackground", background, 101, 1, this);
 	
 	AddEffect("CrankSky", this, 101, 1, this);
}

public func FxCrankSkyTimer()
{
	SetSkyAdjust(HSL2RGB(RGB(GetRGBValue(RGB2HSL(GetSkyAdjust()), 1)+1, 255, 128)));
}

public func FxCrankBackgroundStart(object target, int nr)
{
	EffectVar(0, target, nr) = -1 + Random(2) * 2;
	EffectVar(3, target, nr) = [GetX(target), GetY(target)];
	EffectVar(2, target, nr) = Random(100)+140;
}

public func FxCrankBackgroundTimer(object target, int nr)
{
	EffectVar(1, target, nr) += EffectVar(0, target, nr);
	if(EffectVar(1, target, nr) < 0)
		EffectVar(1, target, nr) = 359;
	else if(EffectVar(1, target, nr) > 359)
		EffectVar(1, target, nr) = 0;
	
	var x = EffectVar(3, target, nr)[0];
	var y = EffectVar(3, target, nr)[1];
	
	SetClrModulation(HSL2RGB(RGB(GetRGBValue(RGB2HSL(GetClrModulation(target)), 1)+1, 255, 128)), target);
	SetPosition(x+Cos(EffectVar(1, target, nr),  EffectVar(2, target, nr)), y+Sin(EffectVar(1, target, nr),  EffectVar(2, target, nr)), target);
}

public func CreateInterior()
{
  return true;
}

public func CreateOptionalFeatures()
{
  //Hintergrund.
  var background = C4Id(Format("BD%02d", Random(9)+1));
  CreateObject(background, LandscapeWidth()/2, LandscapeHeight()+GetDefHeight(background)/3, -1);
}


public func EvalRound()
{
	var players = [];
	for(var obj in FindObjects(Find_ID(PCMK), Find_NoContainer()))
		players[GetLength(players)] = GetOwner(obj);
	
	g_aLastRoundWinners = players;
	return LoadMinigame(MLBW);
}

public func LoadingFinished()
{
	Wait(35*5, -1, true, "StartGame()");
	return true;
}

public func StartGame()
{
	for(var ladr in FindObjects(Find_ID(LADR)))
		AddEffect("LadderPositioning", ladr, 101, Random(8)+1, this);
}

public func FxLadderPositioningStart(object target, int nr)
{
	EffectVar(0, target, nr) = LocalN("horizontal", target);
	
	return true;
}

public func FxLadderPositioningTimer(object target, int nr)
{
	if(EffectVar(0, target, nr))
	{
		target->SetPosition(GetX(target)+1+crankness, GetY(target));
		if(GetX(target) >= LandscapeWidth()-10)
			target->SetPosition(1+crankness, GetY(target));
	}
	else
	{
		target->SetPosition(GetX(target), GetY(target)+1+crankness);
		if(GetY(target) >= LandscapeHeight()-10)
			target->SetPosition(GetX(target), 1+crankness);
	}
	
	return true;
}

//public func StartGame() { FakeScriptGo(true); NextPlayer(); }



