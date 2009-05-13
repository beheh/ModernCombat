/*-- Verteidigen --*/

#strict
#include GOAL

local wave,waves,cooldown,enemys,maxactive,active;

//GameCall("GDEF_Waves"); -> Anzahl der Wellen.
//GameCall(Format("GDEF_MonsterType%d",i)); -> Setup eines Monster-Typs. Returnwert ist das Monster.
//GameCall("GDEF_MonsterSet",i); -> Monster Set. (Array mit MonsterTyp-IDs.) i ist die Welle.
//GameCall("GDEF_MonsterCount",i); -> Monster für Welle i für einen Spieler.
//GameCall("GDEF_MonsterSpawnTime"); -> Wie schnell Monster gespawnt werden können.
//GameCall("GDEF_WaveCooldown",i); -> Cooldownzeit nach Welle i. (In Sekunden!)
//GameCall("GDEF_MaxActive"); -> Maximal gleichzeitig aktive Monster.
//GameCall("GDEF_OnWaveBegin",i); -> Welle i beginnt.
//GameCall("GDEF_OnWaveEnd",i); -> Welle i erfolgreich abgeschlossen.

protected func Initialize()
{
  ScheduleCall(this(),"Initialized",1);
  return(_inherited());
}

public func Initialized()
{
  if(!FindObject(CHOS)) ChooserFinished();
}

public func ChooserFinished()
{
  maxactive = GameCall("GDEF_MaxActive");
  waves = GameCall("GDEF_Waves");
  wave = 1;
  cooldown = 16;
  
  var time = GameCall("GDEF_MonsterSpawnTime");
  if(time)
    AddEffect("IntSpawnMonster",this(),10,time,this());
}

private func GetWaveCount(int iWave)
{
  var m = GameCall("GDEF_MonsterCount",iWave);
  var p = GetPlayerCount();
  return((m*p)-(m-(m/p)));
}

global func IsUserControlled()
{
  if(GetOwner(this()) != NO_OWNER)
  {
    if(GetPlayerType(GetOwner(this())) == C4PT_Script)
      return(false);
    else
      return(true);
  }
  return(false);
}

private func ActiveMosters()
{
  return(ObjectCount2(Find_Func("IsThreat"),Find_OCF(OCF_Alive),Find_Not(Find_Func("IsUserControlled"))));
}

private func ActivePlayers()
{
  return(ObjectCount2(Find_Func("IsThreat"),Find_OCF(OCF_Alive),Find_Func("IsUserControlled"),Find_Not(Find_Func("IsFakeDeath"))));
}

public func Timer()
{
  active = ActiveMosters();

  //Verloren?
  if(!ActivePlayers())
  {
    Message("@Versagt!");

    for(var i=0 ; i < GetPlayerCount() ; i++)
      if(GetPlayerType(GetPlayerByIndex(i)) == C4PT_User)
        EliminatePlayer(GetPlayerByIndex(i));
        
    GameOver(35*2);
        
    return();
  }
  
  //Gewonnen?
  if(IsFulfilled())
  {
    Message("@Überlept!!.");
    GameOver(35*10);
    return();
  }

  //Cooldown?
  if(cooldown > 0)
  {
    cooldown--;
  
    if(!cooldown)
      Message("<c %x>Welle %d hat jetzt begonnen!</c>",0,RGB(255,255),wave);
    else
      Message("@Nächste Welle in <c %x>%02d</c> Sekunden!",0,RGB(255,255),cooldown);
    
    return();
  }
  
  //Welle fertig?
  if((enemys >= GetWaveCount(wave)) && !active)
  {
    if(wave < waves)
    {
      cooldown = GameCall("GDEF_WaveCooldown",wave);
      enemys = 0;
      Log("<c %x>Welle <c %x>%d</c> überstanden!</c>",RGB(0,100),RGB(0,255),wave);
      GameCall("GDEF_OnWaveEnd",wave);
      wave++;
      GameCall("GDEF_OnWaveBegin",wave);
    }
    return();
  }
  
  //Monster spawnen.
  if(!GetEffect("IntSpawnMonster",this()))
    SpawnMonster();
}

private func SpawnMonster()
{
  if((active >= maxactive) || (enemys >= GetWaveCount(wave)) || (cooldown > 0))
    return(false);

  var aSet = GameCall("GDEF_MonsterSet",wave);
  GameCall(Format("GDEF_MonsterType%d",aSet[Random(GetLength(aSet))]));
  //AddEffect("IntGDEF",m,10,35*2,0,GetID(),center_x,center_y);
  enemys++;
  
  return(true);
}

public func FxIntSpawnMonsterTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  SpawnMonster();
}

public func IsFulfilled()
{
  return((wave >= waves) && !active && (enemys >= GetWaveCount(wave)) && GetPlayerCount(C4PT_User));
}

protected func Activate(iPlayer)
{
  return(MessageWindow(GetFulfillText(),iPlayer));
}

private func GetFulfillText()
{
  if(IsFulfilled())
    return("$MsgWon$");
  return(Format("$MsgWave$",wave,waves));
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

/*
public func FxIntGDEFStart(object pTarget, int iEffectNumber, int iTemp, x, y)
{
  EffectVar(0,pTarget,iEffectNumber) = x;
  EffectVar(1,pTarget,iEffectNumber) = y;
}

public func FxIntGDEFTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if((GetCommand(pTarget,0,0) ne "MoveTo") || (GetCommand(pTarget,2,0) != EffectVar(0,pTarget,iEffectNumber)))
    SetCommand(pTarget,"MoveTo",0,EffectVar(0,pTarget,iEffectNumber),EffectVar(1,pTarget,iEffectNumber));
}

public func FxIntGDEFStop(object pTarget, int iEffectNumber, int iReason)
{
  if(iReason != 0) return();
  if(GetCommand(pTarget,0,0) eq "MoveTo")
  {
    if(GetCommand(pTarget,2,0) == EffectVar(0,pTarget,iEffectNumber))
      RemoveEffect("IntGDEFS",pTarget);
  }
}
*/