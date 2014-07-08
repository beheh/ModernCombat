/*-- Lobby --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, NOFF, NOAM]; } 
public func SectionName() { return "SuddenDeath"; } 
public func HideInMenu() { return true; } 
public func IsCMCMinigame() { return true; }
public func FreeForAll() { return true; }

local container;
local theweapon;

public func Initialize()
{ 
	var weapons = [ASTR, RTLR, SGST, RSHL, CDBT];
	theweapon = weapons[Random(GetLength(weapons))];

  container = CreateObject(TIM1);
  g_Gamemaster->Enter(container);
  
  var mx = (LandscapeWidth()-65)/130;
  var my = 3;
  var baum = [];
  for(var i = 0; i < mx; i++)
  {
  	for(var j = 0; j < 3; j++)
  	{
  		var x = 65+130*i; var y = 65+130*j;
  		if(GBackSolid(x, y))
  			continue;
  		
  		if(!GetLength(baum))
  		{
  			baum[0] = [[x,y]];
  			continue;
  		}
  		var fFound = -1;
  		for(var k = 0; k < GetLength(baum); k++)
  		{
  			if(GetPathLength(x, y, baum[k][0][0], baum[k][0][1]))
  			{
  				fFound = k;
  				break;
  			}
  		}
  		
  		if(fFound > -1)
  			baum[fFound][GetLength(baum[fFound])] = [x, y];
  		else
  			baum[GetLength(baum)] = [[x,y]];
  		
  		
  		//baum[GetLength(baum)] = [x,y];
  	}
  }
  
  //Log("%v", baum);
  
  for(var j = 0; j < GetLength(baum); j++)
  {
  	for(var i = j+1; i < GetLength(baum); i++)
 		{
  		if(!GetLength(baum[i]))
  			continue;
  		
  		var point = baum[i][Random(GetLength(baum[i]))];
  		
  		/*while(!(point = baum[i][Random(GetLength(baum[i]))]))
  		{
  			if(!point)
					continue;
  		}*/
  		
  		// Türvebrindung zwischen baum[j][Random] und baum[i][Random] bauen
  		var point1 = baum[j][Random(GetLength(baum[j]))];
  		var doorx = point1[0]; var doory = point1[1]; var txdir, tydir = 1;
  		SimFlight(doorx, doory, txdir, tydir);
  		if(FindObject2(Find_ID(ROOM), Find_Distance(10, doorx, doory)))
  			doorx += 30;
  		
  		var door1 = CreateObject(ROOM, doorx, doory, -1);
  		
  		var doorx = point[0]; var doory = point[1]; var txdir, tydir = 1;
  		SimFlight(doorx, doory, txdir, tydir);
  		if(FindObject2(Find_ID(ROOM), Find_Distance(10, doorx, doory)))
  			doorx += 30;
  		
  		var door2 = CreateObject(ROOM, doorx, doory, -1);
  		
  		
  		door1->Connect(door2);
  	}
  }
  
  for(var i = 0; i < Random(10)+10; i++)
  {
  	var twipf = PlaceAnimal(WIPF);
  	CreateObject(BSH2, GetX(twipf), GetY(twipf)+20, -1);
  	RemoveObject(twipf);
  }
  return _inherited(...);
}

public func InitializePlayer(int iPlr)
{
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];

	var wipf = PlaceAnimal(WIPF);
	
	SetRespawnPosition(GetX(wipf), GetY(wipf), iPlr);
	RemoveObject(wipf);
	
	return _inherited(iPlr, ...);
}

public func OnClonkRelaunch(object pCrew, bool fInitPlr) 
{
	if(!fInitPlr)
	{
		pCrew->Enter(container);
		if(ObjectCount2(Find_ID(PCMK), Find_NoContainer())-1 <= 1)
			return SetDelayedCallObj(DelayedCall("EvalRound", 1), this); 
	}
	else
	{
		// Jetpack hinzufügen.
		CreateContents(theweapon, pCrew);
		CreateObject(JTPK, 0, 0, GetOwner(pCrew))->~Activate(pCrew);
		DoEnergy(-149, pCrew);
	}
	
	return true;
}

public func CreateInterior()
{
  return true;
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
	
}

