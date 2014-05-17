/*-- Killstats --*/

#strict 2
#appendto KILL

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
      typeicon = SM04;
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
  
  //Attachment? Dann dranschreiben!
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
    
    //Killer selbst tot?
    if(Contained(GetCursor(killer)) && GetID(Contained(GetCursor(killer))) == FKDT)
      energystr = "({{SM01}})";
    else
      energystr = Format("({{SM13}} <c ff0000>%d%</c>)", (GetEnergy(GetCursor(killer))*100/(GetPhysical("Energy", 0, GetCursor(killer))/1000)));
    
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
