/*-- Killstatistiken --*/

#strict

static const STAT_Spree = 5;


public func KTMsg(int plr1, int plr2, object clonk, int plr3)
{
  if(!plr1 && !plr2)
    return();

  if(!GetPlayerName(plr1) || !GetPlayerName(plr2))
    return();

  KMsg(plr1,plr2,clonk,plr3);
}

/* Killnachricht */

public func KMsg(int plr1, int plr2, object clonk, int plr3)
{
  var tp;
  for(var goal in FindObjects(Find_Category(C4D_Goal)))
    if(goal->~IsTeamGoal())
      tp = true;
      
  if(!tp)
    return();

  //Kein Clonk?
  if(!clonk) return();
	if(!GetPlayerName(plr1)) return;

  var msg;
  var typeicon,type = clonk->~LastDamageType();

  //Icon setzen
  var killicon = clonk->~KillIcon();
  if(!killicon) killicon = SKUL;

  if(type)
  {
   if(type == DMG_Fire)
    typeicon = GSAM;
   else if(type == DMG_Melee)
   	typeicon = ICFT;
   else if(type == DMG_Explosion)
    typeicon = BOOM;
   else if(type == DMG_Energy)
    typeicon = ENAM;
   else if(type == DMG_Bio)
    typeicon = GLOB;
   else if(type == DMG_Projectile)
    if(killicon)
      if(killicon->~IsWeapon())
      	typeicon = SHTX;
      else
      	typeicon = SHRP;
  }

  //Kein Icon?
  if(!killicon && !typeicon) typeicon = SKUL;

  //Killer links
  var victim = plr1;
  var killer = plr2;
  var assist = plr3;

	//Nachricht konstruieren
  msg = Format("{{%i}}",killicon); 
  if(typeicon && killicon != typeicon)
    if(killicon)
    	msg = Format("%s ({{%i}})",msg,typeicon);
    else
			msg = Format("%s{{%i}}",msg,typeicon);    	
  msg = Format("%s %s", msg, GetTaggedPlayerName(victim));
  if(killer != victim) {
  	
	  var killerstr;
	  if(assist != -1 && assist != killer && assist != victim) {
	  	killerstr = Format("%s +<c %x> %s</c>", GetTaggedPlayerName(killer), RGB(180,180,180), GetPlayerName(assist));
	  }
	  else {
	  	killerstr = GetTaggedPlayerName(killer);
	  }
	  msg = Format("%s %s", killerstr, msg);
	}
  
  //Eventnachricht: Spieler eliminiert Spieler
  EventInfo4K(0,msg,0);

  //Und an den Toten weiterleiten
  if (Contained(clonk) && !FindObject(NOFD))
    Contained(clonk)->~KillMessage(msg);
}

/* Selbstmordnachricht */

public func SKMsg(int plr, object clonk)
{
  KMsg(plr,plr,clonk);
}

/* Teamkillnachricht */

public func TKMsg()
{
  KMsg(...);
}

/* Spielzielpunkt */

public func SMsg(int plr)
{
  //Eventnachricht: Spieler erhält Punkt
  EventInfo4K(0,Format("$MsgScore$",GetTaggedPlayerName(plr),GetPlrTeamName(plr)),PCMK);
}

private func GetPlrTeamName(int plr)
{
  return(Format("<c %x><%s></c>", GetTeamColor(GetPlayerTeam(plr)),GetTeamName(GetPlayerTeam(plr))));
}

/* Killstatistiken */

public func KillStat(object pClonk, int killedplr)
{
  //nein, die Engine darf keine Kills machen. >:(
  if(!pClonk)
   if(!(pClonk = this()))
    return();
  //Nein, Teamkills bringen keine Punkte. :(
  if(GetPlayerTeam(killedplr) == GetPlayerTeam(GetController(pClonk)))
   return();
  AddEffect("KillStats",pClonk,23,10,this(),HHKS);
}

//KillStats-Effekt, Tötungsstatistiken!
func FxKillStatsStart(object pTarget, int iEffectNumber, int iTemp)
{
  if(iTemp)
   return(FX_OK);
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
func FxKillStatsTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Haben wir gerade Killingtime?
  if(!EffectVar(3, pTarget, iEffectNumber))
   return();
  //Schon lange genug gewartet?
  if(iEffectTime - EffectVar(1, pTarget, iEffectNumber) > 126) { //Die dreifache Wahrheit! 
   EffectVar(2, pTarget, iEffectNumber) = 0;
   EffectVar(3, pTarget, iEffectNumber) = 0;
  }
}

//Auswertung der Statistik beim Tod
func FxKillStatsStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  //Auswerten?
  if(iReason != 3 && iReason != 4)
   return();

  if(EffectVar(0, pTarget, iEffectNumber) >= STAT_Spree)
   if(GetName(pTarget))
    Log("$MsgSpreeEnd$", GetName(pTarget));
}

//Kill++
func FxKillStatsEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber)
{
  if(szNewEffectName S= "KillStats")
   return(-3);
}

func FxKillStatsAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer)
{
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