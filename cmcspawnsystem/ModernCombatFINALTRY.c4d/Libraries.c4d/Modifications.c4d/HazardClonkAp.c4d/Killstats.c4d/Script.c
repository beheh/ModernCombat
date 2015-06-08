/*-- Killstatistiken --*/

#strict 2
#appendto HHKS

static const STAT_Spree = 5;


public func KTMsg(int plr1, int plr2, object clonk, int plr3)
{
  if(!plr1 && !plr2)
    return;

  if(!GetPlayerName(plr1) || !GetPlayerName(plr2))
    return;

  KMsg(plr1,plr2,clonk,plr3);
}

/* Killnachricht */

public func KMsg(int plr1, int plr2, object clonk, int plr3)
{
  //Kein Clonk?
  if(!clonk) return;
  if(!GetPlayerName(plr1)) return;

  var msg;
  var typeicon,type = clonk->~LastDamageType();

  //Icon setzen
  var killicon = clonk->~KillIcon();
  var killattachment = clonk->~KillAttachment();
  if(!killicon) killicon = SKUL;

  if(type)
  {
    if(type == DMG_Fire)
      typeicon = GSAM;
    else if(type == DMG_Melee)
    {
      if(GetID(LocalN("pLastObjectHit", clonk)) == killicon)
        typeicon = SM25;
      else
        typeicon = SM04;
    }
    else if(type == DMG_Explosion)
      typeicon = BOOM;
    else if(type == DMG_Energy)
      typeicon = ENAM;
    else if(type == DMG_Bio)
      typeicon = GLOB;
    else if(type == DMG_Projectile)
      if(killicon)
        if(killicon->~IsWeapon() || killicon->~HasBullets())
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

  //Attachment vorhanden: Hinzufügen
  if(killattachment && killicon->~IsWeapon())
    msg = Format("%s{{%i}}", msg, AttachmentIcon(killattachment));
  if(typeicon && killicon != typeicon)
    if(killicon)
    {
      msg = Format("%s ({{%i}})",msg,typeicon);
    }
    else
    {
      msg = Format("%s{{%i}}",msg,typeicon);
  	}
  
  msg = Format("%s %s", msg, GetTaggedPlayerName(victim));
  var dmsg = msg;
  if(killer != victim)
  {
    var killerstr, dstr, energystr;
    var cursor = GetCursor(killer);
    if(cursor->~GetRealCursor())
      cursor = cursor->GetRealCursor();

    //Killer selbst tot?
    if(Contained(GetCursor(killer)) && GetID(Contained(cursor)) == FKDT)
      energystr = "({{SM01}})";
    else
      energystr = Format("({{SM13}} <c ff0000>%d%</c>)", (GetEnergy(cursor)*100/(GetPhysical("Energy", 0, cursor)/1000)));

    if(assist != -1 && GetPlayerName(assist) && assist != killer && assist != victim)
    {
      dstr = Format("%s %s + <c %x>%s</c>", GetTaggedPlayerName(killer), energystr, RGB(180,180,180), GetPlayerName(assist));
      killerstr = Format("%s + <c %x>%s</c>", GetTaggedPlayerName(killer), RGB(180,180,180), GetPlayerName(assist));
    }
    else
    {
      killerstr = GetTaggedPlayerName(killer);
      dstr = Format("%s %s", GetTaggedPlayerName(killer), energystr);
    }
    dmsg = Format("%s %s", dstr, msg);
    msg = Format("%s %s", killerstr, msg);
  }

  //Eventnachricht: Spieler eliminiert Spieler
  EventInfo4K(0,msg,0);

  //Und an den Toten weiterleiten
  if(Contained(clonk) && !FindObject(NOFD))
    Contained(clonk)->~KillMessage(dmsg);
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
  //Engine keine Kills zuschreiben
  if(!pClonk)
    if(!(pClonk = this()))
      return;
  //Teamkills keine Punkte zuschreiben
  if(GetPlayerTeam(killedplr) == GetPlayerTeam(GetController(pClonk)))
    return;
  AddEffect("KillStats",pClonk,23,10,this(),HHKS);
}

/* KillStats-Effekt und Tötungsstatistiken */

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

/* Prüfung auf Killerzeit */

public func FxKillStatsTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Killerzeit vorhanden?
  if(!EffectVar(3, pTarget, iEffectNumber))
    return;
  //Genug gewartet?
  if(iEffectTime - EffectVar(1, pTarget, iEffectNumber) > 126)
  {
    EffectVar(2, pTarget, iEffectNumber) = 0;
    EffectVar(3, pTarget, iEffectNumber) = 0;
  }
}

/* Auswertung bei Tod */

public func FxKillStatsStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(iReason != 3 && iReason != 4)
    return;
}

/* Kill hinzurechnen */

public func FxKillStatsEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber)
{
  if(szNewEffectName == "KillStats")
   return(-3);
}

public func FxKillStatsAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer)
{
  //Neue Daten speichern
  ++EffectVar(0, pTarget, iEffectNumber);
  var running = ++EffectVar(2, pTarget, iEffectNumber);
  EffectVar(1, pTarget, iEffectNumber) = GetEffect("KillStats", pTarget, 0, 6);
  EffectVar(3, pTarget, iEffectNumber) = 1;

  //auf Multikills prüfen
  if(running >= 2)
  {
    var msg = $MsgMultikill$;
    Message("<c ff0000>%s</c> (%d)",pTarget, msg[Min(running-2,GetLength(msg)-1)], running);

    //Punkte bei Belohnungssystem (Kill-Serie)
    DoPlayerPoints(BonusPoints("KillSeries"), RWDS_BattlePoints, GetOwner(pTarget), pTarget, IC01);

    if(running == 3)
      //Achievement-Fortschritt (Third Time Lucky)
      DoAchievementProgress(1, AC31, GetOwner(pTarget));
  }

  return true;
}