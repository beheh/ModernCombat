/*-- Zombiejagd --*/

#strict
#include GOAL

public func IsFulfilled()
{
  return(!GetLength(ZombiesLeft()));
}

protected func Activate(iPlayer)
{
  return(MessageWindow(GetFulfillText(),iPlayer));
}

private func GetFulfillText()
{
  var aZombies = ZombiesLeft();
  if(!GetLength(aZombies))
    return("$MsgAllDead$");
  var zombies = CreateArray();
  for(var obj in aZombies)
  {
    if(!FindIn(zombies,GetID(obj)))
      zombies[GetLength(zombies)] = GetID(obj);
  }

  var text = "$MsgStillLeft$";
  for(var ID in zombies)
  {
    text = Format("%s|{{%i}} %s: %d",text,ID,GetName(0,ID),ObjectCount(ID));
  }
  return(text);
}

private func ZombiesLeft()
{
	return(FindObjects(Find_Faction(FACTION_Xen)));
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

private func FindIn(array a, b)
{
  for(var c in a)
    if(c == b)
      return(true);
      
  return(false);
}
