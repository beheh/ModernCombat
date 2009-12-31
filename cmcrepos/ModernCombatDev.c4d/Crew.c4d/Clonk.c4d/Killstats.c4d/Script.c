/*-- Killstatistics --*/

#strict 2

static const STAT_Spree = 5;

public func KTMsg(int plr1, int plr2, object clonk)
{
  if(!plr1 && !plr2)
    return ;

  if(!GetPlayerName(plr1) || !GetPlayerName(plr2))
    return ;

  KMsg(plr1,plr2,clonk);
}

//Kill - Spieler A hat Spieler B getötet!
public func KMsg(int plr1, int plr2, object clonk)
{
  /* Für Debugzwecke temporär entfernt.
  var tp;
  for(var goal in FindObjects(Find_Category(C4D_Goal)))
    if(goal->~IsTeamGoal())
      tp = true;
      
  if(!tp)
    return ;*/
    
  if(!clonk) return ;

  var msg;
  var typeicon,type = clonk->~LastDamageType();
  
  if(type)
  {
    if(type == DMG_Fire)
      typeicon = GSAM;
    else if(type == DMG_Explosion)
      typeicon = BOOM;
    else if(type == DMG_Energy)
      typeicon = ENAM;
    else if(type == DMG_Bio)
      typeicon = GLOB;
    else if(type == DMG_Projectile)
      typeicon = STAM;
  }
  /*else
    typeicon = SKUL;*/
  
  
  
  var killicon = clonk->~KillIcon();
  if(!killicon) killicon = SKUL;
  
  msg = Format("%s {{%i}}",GetTaggedPlayerName(plr1),killicon);
  if(typeicon)
    msg = Format("%s({{%i}})",msg,typeicon);
  
  if(plr1 != plr2)
    msg = Format("%s %s",msg,GetTaggedPlayerName(plr2));
    
  //Log(msg);//TODO: Hier kann Nachrichten-System ansetzen. ;>
  EventInfo4K(0,msg);//:>
}

//Selfkill - Spieler A hat sich selbst umgebracht! D:
public func SKMsg(int plr, object clonk)
{
  KMsg(plr,plr,clonk);
}

//Teamkill - Spieler von Team A hat Spieler des selben Teams getötet! :O
public func TKMsg()
{
  KMsg(...);
}

//Score - Spieler von Team A hat gepunktet!
public func SMsg(int plr)
{
  Log("$MsgScore$",GetTaggedPlayerName(plr),GetPlrTeamName(plr));
}

private func GetPlrTeamName(int plr)
{
  return(Format("<c %x><%s></c>", GetTeamColor(GetPlayerTeam(plr)),GetTeamName(GetPlayerTeam(plr))));
}







/* Killstatistiken */

public func KillStat(object pClonk, int killedplr) {
  //nein, die Engine darf keine Kills machen. >:(
  if(!pClonk)
    if(!(pClonk = this))
      return ;
  //Nein, Teamkills bringen keine Punkte. :(
  if(GetPlayerTeam(killedplr) == GetPlayerTeam(GetController(pClonk)))
    return ;
  AddEffect("KillStats",pClonk,23,10,this,HHKS);
}

//KillStats-Effekt, Tötungsstatistiken!
func FxKillStatsStart(object pTarget, int iEffectNumber, int iTemp) {
  if(iTemp)
    return FX_OK;
  //Effectvars:
  // 0 - Anzahl der Kills seit dem letzten Tod
  // 1 - Zeitpunkt des letzten Kills (relativ)
  // 2 - Anzahl der Kills innerhalb einer bestimmten Zeitperiode (Doublekill, etc.)
  // 3 - Ob gerade ein Kill war, und die Zeit noch läuft
  EffectVar(0, pTarget, iEffectNumber) = 0;
  EffectVar(1, pTarget, iEffectNumber) = 0;
  EffectVar(2, pTarget, iEffectNumber) = 0;
  FxKillStatsAdd(pTarget, iEffectNumber);
}

//Schaun ob die Killzeit schon abgelaufen ist
func FxKillStatsTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  //Haben wir gerade Killingtime?
  if(!EffectVar(3, pTarget, iEffectNumber))
    return ;
  //Schon lange genug gewartet?
  if(iEffectTime - EffectVar(1, pTarget, iEffectNumber) > 126) { //Die dreifache Wahrheit! 
    EffectVar(2, pTarget, iEffectNumber) = 0;
    EffectVar(3, pTarget, iEffectNumber) = 0;
  }
}

//Auswertung der Statistik beim Tod
func FxKillStatsStop(object pTarget, int iEffectNumber, int iReason, bool fTemp) {
  //Auswerten?
  if(iReason != 3 && iReason != 4)
    return ;
  
  if(EffectVar(0, pTarget, iEffectNumber) >= STAT_Spree)
    if(GetName(pTarget))
      Log("$MsgSpreeEnd$", GetName(pTarget));
}

//Kill++
func FxKillStatsEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName == "KillStats")
    return -3;
}

func FxKillStatsAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer) {
  //Neue Daten speichern
  var total = ++EffectVar(0, pTarget, iEffectNumber);
  var running = ++EffectVar(2, pTarget, iEffectNumber);
  EffectVar(1, pTarget, iEffectNumber) = GetEffect("KillStats", pTarget, 0, 6);
  EffectVar(3, pTarget, iEffectNumber) = 1;
  
  //Auf Killfolgen prüfen
  var msg = $MsgKillingSpree$;
  var t = GetLength(msg);
  if(!(total%STAT_Spree) && total/STAT_Spree <= t) {
    Log(Format("%s (%s)", msg[Min(total/STAT_Spree-1,t-1)],GetName(pTarget)));
  }

  //auf Multikills prüfen
  if(running >= 2) {
    msg = $MsgMultikill$;
    Message("<c ff0000>%s</c>",pTarget, msg[Min(running-2,GetLength(msg)-1)]);
  }
}



