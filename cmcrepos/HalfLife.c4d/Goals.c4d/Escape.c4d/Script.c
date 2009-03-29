/*-- Escape --*/

#strict
#include GOAL

//GameCall("GESC_Escaped",pCrew); -> Spieler ist entkommen?
//GameCall("GESC_Success"); -> Alle Spieler entkommen! Kann z.B. für irgendwelche Animationen usw. genutzt werden.

//GameCall("GESC_ResupplyPlaces"); -> Anzahl der sicheren Räume.
//GameCall(Format("GESC_ResupplyPlace%d",i)); -> Sicheren Raum erstellen. (D.h. Item-Spawns usw. einrichten.)

//GameCall("GESC_MonsterSpawns"); -> Anzahl der Monster-Spawnpunkte
//GameCall(Format("GESC_MonsterSpawnX%d",i)); -> X-Position
//GameCall(Format("GESC_MonsterSpawnY%d",i)); -> Y-Position
//GameCall(Format("GESC_MonsterSpawn%d",i)); -> Bei Monster-Spawnpunkt i ein Monster spawnen.
//GameCall(Format("GESC_MonsterSpawnFull%d",i)); -> Genug Monster? (In Prozent angegeben.)

local success;
local aSpawnX;
local aSpawnY;
local aActiveSpawns;

protected func Initialize()
{
  aActiveSpawns = [];
  aSpawnX = [];
  aSpawnY = [];
  success = false;
  ScheduleCall(this(),"Initialized",1);
  return(_inherited());
}

public func Initialized()
{
  if(!FindObject(CHOS)) ChooserFinished();
}

public func ChooserFinished()
{
  var checked = [];
  var max = 0;
  var count = 0;
  var safeme = 0;
  var rnd = 0;

  
  //Resupply-Places
  
  max = GameCall("GESC_ResupplyPlaces");
  if(max)//Gibt es die überhaupt?
  {
    checked = CreateArray(max);
    count = Max(max/2,1);
    
    while(count--)
    {
      safeme = 10;
      while(safeme-- > 0)
      {
        rnd = Random(max);
        if(!checked[rnd]) break;
      }
      if(safeme <= 0) break;
      
      GameCall(Format("GESC_ResupplyPlace%d",rnd+1));
      checked[rnd] = true;
    }
  }
  
  
  //Monster-Spawns
  max = GameCall("GESC_MonsterSpawns");
  if(max)
  {
    for(var i = 0; i < max; i++)//Positionen einlesen. :)
    {
      aSpawnX[i] = GameCall(Format("GESC_MonsterSpawnX%d",i+1));
      aSpawnY[i] = GameCall(Format("GESC_MonsterSpawnY%d",i+1));
    }
    
    aActiveSpawns = CreateArray(max);
    count = Max(max/2,1);
    
    while(count--)
    {
      safeme = 10;
      while(safeme-- > 0)
      {
        rnd = Random(max);
        if(!aActiveSpawns[rnd]) break;
      }
      if(safeme <= 0) break;

      aActiveSpawns[rnd] = true;
    }
    
    DoMonsters(30,2);
  }
}

protected func Timer()
{
  if(success) return();

  if(!Random(12))
    DoMonsters(30,5);
    
  if(!Random(4))
  {
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      var plr = GetPlayerByIndex(i);
      if(GetPlayerTeam(plr) >= 2) continue;

      var j = GetCrewCount(plr);
      while(j--)
        DoPlrMonsters(GetCrew(plr,j),200,100,3);
    }
  }
}

private func DoMonsters(int iMaxFullness, int iRandomFactor)
{
  if(!iMaxFullness)//Was ist das für ein Wort? O_o
    iMaxFullness = 100;

  var i = GetLength(aActiveSpawns);
  if(!i) return();
  while(i--)
  {
    if(!aActiveSpawns[i]) continue;
    
    if(iRandomFactor) if(Random(iRandomFactor)) continue;
    
    if(GameCall(Format("GESC_MonsterSpawnFull%d",i+1)) > iMaxFullness)
      continue;
      
    GameCall(Format("GESC_MonsterSpawn%d",i+1));
  }
}

private func DoPlrMonsters(object pCrew, int iRange, int iMaxFullness, int iRandomFactor)
{
  if(!pCrew) return();

  if(!iMaxFullness)//Was ist das für ein Wort? O_o
    iMaxFullness = 100;
  
  if(!iRange)
    iRange = 300;

  var i = GetLength(aActiveSpawns);
  if(!i) return();
  while(i--)
  {
    if(!aActiveSpawns[i]) continue;
    
    if(Distance(GetX(pCrew),GetY(pCrew),aSpawnX[i],aSpawnY[i]) > iRange) continue;
    
    if(iRandomFactor) if(Random(iRandomFactor)) continue;
    
    if(GameCall(Format("GESC_MonsterSpawnFull%d",i+1)) > iMaxFullness)
      continue;
      
    GameCall(Format("GESC_MonsterSpawn%d",i+1));
  }
}

public func IsFulfilled()
{
  var valid = false;

  for(var i = 0; i < GetPlayerCount(); i++)
  {
    if(GetPlayerTeam(GetPlayerByIndex(i)) >= 2) continue;//Monster Team fällt weg.
    
    valid = true;
    var j = GetCrewCount(GetPlayerByIndex(i));
    while(j--)
    {
      if(!GameCall("GESC_Escaped",GetCrew(GetPlayerByIndex(i),j)))
        return(false);
    }
  }

  if(valid)
  {
    if(!success)//Sicherstellen, das das nur einmal aufgerufen wird.
    {
      success = true;
      GameCall("GESC_Success");
    }
    return(true);
  }
  else
    return(false);
}

protected func Activate(iPlayer)
{
  return(MessageWindow(GetFulfillText(),iPlayer));
}

private func GetFulfillText()
{
  if(IsFulfilled())
    return("$MsgEscaped$");
  
  var text = "$MsgPlrInfo$";
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    var plr = GetPlayerByIndex(i);
    if(GetPlayerTeam(plr) >= 2) continue;
    
    text = Format("%s|%s:",text,GetTaggedPlayerName(plr));
    
    var j = GetCrewCount(plr);
    while(j--)
    {
      var crew = GetCrew(plr,j);
      var cur = crew->GetEnergy();
      var max = crew->GetPhysical("Energy")/1000;
      text = Format("%s|  <c %x>%s: </c><c %x>%03d HP</c>",text,GetPlrColorDw(plr),GetName(crew),RGB(255-(cur*255/max),cur*255/max),cur);
    }
  }

  return(text);
}

public func MonsterKilled(id idMonster, object pMonster, int iKiller)
{
  var value;
  if(pMonster)
    value = GetValue(pMonster,0,0,iKiller);
  else
    value = GetValue(0,idMonster,0,iKiller);

  DoWealth(iKiller,value);
}
