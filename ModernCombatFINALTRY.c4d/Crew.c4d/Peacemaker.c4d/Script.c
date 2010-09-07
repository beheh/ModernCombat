/*--- Der Peacemaker ---*/

#strict 2
#include HZCK

#include L_A2 //Verfügt über CMC Agilität
#include L_ND //Kann Handgranaten einsetzen
#include L_CR //Kann Liegen und Kriechen

public func WeaponCollectionLimit() { return 3; } //Der Clonk kann drei Waffen tragen
public func ObjectCollectionLimit() { return 2; } //Und 2 beliebige Zusatzobjekte


/* Initialisierung */

public func Initialize() 
{
  UpdateAmmoBars();
  return _inherited();
}

public func CanUse(id idObj)
{
  if(idObj == HARM) return;
  return _inherited(idObj,...);
}

private func AbortWalk()
{
  if(GetAction() == "Dive")
  {
    var c = Contents();
    if(c)
    {
      if((c->~IsWeapon() && !c->~GetFMData(FM_Aim)) || c->~IsGrenade())
        SetAction("Jump");
    }
  }
}

protected func ContactBottom()
{
  return;
}

protected func UpdateAmmoBars()
{
  if(FindObject(NOAM))
  {
    RemoveAll(ABAR);
  }
  else
  {
    for(var clonk in FindObjects(Find_OCF(OCF_Alive), Find_OCF(OCF_CrewMember), Find_NoContainer(), Find_Exclude(this)))
    {
      var breaks = false;
      for(var bar in FindObjects(Find_ID(ABAR), Find_ActionTarget(clonk)))
      {
        if(GetOwner(bar) == GetOwner())
        {
          if(!Contents() || GetID(Contents()) != AMPK)
          {
            RemoveObject(bar);
          }
          else
          {
            bar->SetBarCount(GetLength(FindObjects(Find_Func("IsBar"), Find_Func("BarActive"), Find_ActionTarget(clonk), Find_Owner(GetOwner()))));
          }
          breaks = true;
        }
      }
      if(breaks) continue;
      if(!Contents() || GetID(Contents()) != AMPK) continue;
      if(Hostile(GetOwner(clonk),GetOwner())) continue;
      if(!GetPlayerName(GetController(clonk))) continue;
      CreateObject(ABAR,AbsX(GetX(clonk)),AbsY(GetY(clonk)),GetOwner())->Set(clonk,RGB(255,255,80),GetLength(FindObjects(Find_Func("IsBar"), Find_Func("BarActive"), Find_ActionTarget(clonk), Find_Owner(GetOwner())))+1);
    }
  }
  ScheduleCall(0,"UpdateAmmoBars",1);
}

/* KI-Erweiterung */

public func GetReanimationTarget(pFrom, &body, &defi, fUnderAttack) {
	var distance = 200;
	if(fUnderAttack) distance = 50;
	body = FindObject2(Find_Func("IsFakeDeath"), Find_Category(C4D_Living), Find_Distance(distance, AbsX(GetX(pFrom)), AbsY(GetY(pFrom))), Find_Allied(GetOwner(pFrom)), Sort_Distance(AbsX(GetX(pFrom)), AbsY(GetY(pFrom))));
	if(body) {
		if(ContentsCount(CDBT, pFrom)) {
			defi = FindObject2(Find_ID(CDBT), Find_Container(pFrom));
			if(!defi->~Ready()) {
				pFrom->~DropObject(defi);
				defi = false;
			}
		}
		if(!defi) defi = FindObject2(Find_ID(CDBT), Find_Or(Find_Container(Contained(body)), Find_Container(pFrom), Find_NoContainer()), Find_Func("Ready"), Find_Distance(distance, AbsX(GetX(pFrom)), AbsY(GetY(pFrom))), Sort_Distance(AbsX(GetX(pFrom)), AbsY(GetY(pFrom))));
		if(defi) {
			return true;
		}
	}
	body = false;
	return false;
}

public func FxAggroTimer(object pTarget, int no)
{
  if(Contained()) return;
	// Erst mal schauen ob wir dringend helfen können
	var body, defi;
	GetReanimationTarget(pTarget, body, defi, EffectVar(1, this(), no)); //Checkt auch nach Defi
	if(body) {
		if(IsAiming()) StopAiming();
		if(Contents() == defi && GetProcedure(pTarget) && ObjectDistance(body, pTarget) < 10) {
			ScheduleCall(defi, "Activate", 1, 0, pTarget);
		}
		else {
			if(!Contained(defi) || Contained(defi) != pTarget) {
				if(GetCommand(pTarget) != "Get") SetCommand(pTarget, "Get", defi);
			}
			else if(Contents() != defi) { //Nicht ausgewählt?
				ShiftContents(pTarget, 0, CDBT);
			}
			else {
				SetMacroCommand(pTarget, "MoveTo", body, 0,0,0, EffectVar(0, pTarget, no));
			}
			return(1);
		}
	}
	// Verletzt?
	if(!pTarget->~IsHealing() && pTarget->GetEnergy() < pTarget->GetPhysical("Energy") * 2/3 / 1000) {
		// Medic, FAP dabei und kein Dragnin? Entpacken!
		if(!ContentsCount(DGNN, pTarget) && ContentsCount(FAPK, pTarget)) {
			var pFAP = FindObject2(Find_ID(FAPK), Find_Container(pTarget), Find_Func("CanUnpack", pTarget));
			if(pFAP)
				pFAP->ControlThrow(pTarget);
		}
		// Und schnell mal nach Dragnin suchen
		var pDragnin = FindObject2(Find_ID(DGNN), Find_Container(pTarget));
			if(pDragnin) pDragnin->Activate(pTarget);
	}
	// Okay - und sonst noch wer in meiner Nähe stark verletzt?
	//var friends;
	for(var friend in FindObjects(Find_Category(OCF_Living), Find_NoContainer(), Find_Distance(50, AbsX(GetX(pTarget)), AbsY(GetY(pTarget))), Sort_Distance())) {
		if(!friend->~IsHealing() && friend->GetEnergy() < friend->GetPhysical("Energy") * 1/3 / 1000) {
			// Medic, FAP dabei und kein Dragnin? Entpacken!
			if(!ContentsCount(DGNN, pTarget) && ContentsCount(FAPK, pTarget)) {
				var pFAP = FindObject2(Find_ID(FAPK), Find_Container(pTarget), Find_Func("CanUnpack", pTarget));
				if(pFAP)
					pFAP->ControlThrow(pTarget);
			}
			// Und schnell mal nach Dragnin suchen
			var pDragnin = FindObject2(Find_ID(DGNN), Find_Container(pTarget));
			if(pDragnin) {
				if(ObjectDistance(friend, pTarget) < 10) {
					pDragnin->ControlThrow(pTarget);
				}
				else {
					SetMacroCommand(pTarget, "MoveTo", friend, 0,0,0, EffectVar(0, pTarget, no));
				}
			}
			break;
		}
	}
  // Wir haben ein Ziel?
  if(EffectVar(1, this, no) && (pTarget->~GetSpread() < 80 || ObjectDistance(pTarget, EffectVar(1, this, no))) < 30) { EffectCall(this(), no, "Fire"); return 1; }
  // Zielen beenden
  if(IsAiming()) StopAiming();
//  Message("@No target", this());
  // Ziel suchen
  var dir = GetDir()*2-1;
  // Vorne
  var target = GetTarget(90*dir, 90);
  // Hinten
  if(!target)
    if((!GetCommand() && !GetMacroCommand()) || EffectVar(0, this(), no) != 1)
      target = GetTarget(-90*dir, 90);
  // Gefunden?
  if(!target)
  {
  	// Nichts gefunden :(
    // -> Bescheid geben!
    if(EffectVar(99, this(), no))
    {
      if(Contained())
        Contained()->~HandleAggroFinished(this());
      else if(IsRiding())
        GetActionTarget()->~HandleAggroFinished(this());
      
      EffectVar(99, this(), no);
    }
    // -> Waffen durchchecken
    CheckIdleWeapon();
    return;
  }
  // Super
  EffectVar(1, this(), no) = target;
  EffectVar(99,this(), no) = true; // wir haben ein Ziel \o/
}

// Wenn iLevel = 1 (Aggro_Shoot) werden keine Waffen mit FM_Aim ausgewählt
public func SelectWeapon(int iLevel, object pTarget, bool fFireModes)
{
  // Entfernung zum Ziel
  var dist = ObjectDistance(pTarget);
  // Keine Waffen in Inventar?
  if(!CustomContentsCount("IsWeapon")) return;
  // Bevorzugten Schadenstyp bestimmen
  var preftype = GetPrefDmgType(pTarget), type;
  // Alle durchgehen und passende prüfen
  for(var i=0,obj,fav,mode,favmode ; obj = Contents(i) ; mode++)
  {
    // Nix Waffe
    if(!(obj->~IsWeapon())) { i++; mode = -1; continue; }
    // Feuermodus
    if(mode && !fFireModes) { i++; mode = -1; continue; }
    if(!(obj->GetFMData(FM_Name, mode))) { i++; mode = -1; continue; }
    if(mode == obj->GetFireMode() && mode) continue;
    // Nix gut
    if(obj->GetFMData(FM_Aim, mode)>0)
      if(iLevel == 1 || !WildcardMatch(GetAction(), "*Walk*"))
        continue;
    // Keine Munition dafür?
    if(!NoAmmo() && !(obj->GetCharge()) && !GetAmmo(obj->GetFMData(FM_AmmoID, mode)))
      continue;
    // EMP nur gegen Maschinen
    if(obj->GetBotData(BOT_EMP, mode))
      if(!(pTarget->~IsMachine()))
        continue;
    // Kein Favorit bisher?
    if(!fav)
    {
      fav = obj;
      type = fav->GetBotData(BOT_DmgType, mode);
      favmode = mode;
    }
    else
    {
      // Favorit hat nicht genug Reichweite
      if(fav->GetBotData(BOT_Range, favmode) < dist)
      {
        // Neue Waffe hat mehr
        if(obj->GetBotData(BOT_Range, mode) > dist)
        {
          fav = obj;
          type = obj->GetBotData(BOT_DmgType, mode);
          favmode = mode;
        }
      }
      else
      {
        // Favorit hat genug Reichweite -> nur wechseln, wenn Schadenstyp besser
        if(pTarget->~OnDmg(obj->GetBotData(BOT_DmgType, mode)) < pTarget->~OnDmg(type) &&
          // Allerdings darf die Waffe nicht zu schwach sein
          fav->GetBotData(BOT_Power, favmode)-1 <= obj->GetBotData(BOT_Power, mode))
        {
          // Neuer Favorit
          fav = obj;
          type = fav->GetBotData(BOT_DmgType);
          favmode = mode;
        }
        else
        {
          // Stärke der neuen Waffe ist größer oder Favorit ist ein Langlader
          if(fav->GetBotData(BOT_Power, favmode) < obj->GetBotData(BOT_Power, mode) ||
             (fav->GetBotData(BOT_Power, favmode) == BOT_Power_LongLoad && (fav->IsReloading() || !(fav->GetCharge()))))
          {
            // Waffe hat keine extralange Nachladezeit
            if(obj->GetBotData(BOT_Power, mode) != BOT_Power_LongLoad)
            {
              // Neuer Favorit
              fav = obj;
              type = fav->GetBotData(BOT_DmgType);
              favmode = mode;
            }
            // Waffe sollte nicht nachladen und nicht leer sein
            else if(obj->GetCharge() != 0 && !(obj->IsReloading()))
              {
                // Neuer Favorit
                fav = obj;
                type = fav->GetBotData(BOT_DmgType);
                favmode = mode;
              }
          }
        } 
      }
      // Reichweite passt
      if(fav->GetBotData(BOT_Range, favmode) >= dist)
        // Schadenstyp auch
        if(preftype == type)
          // Stärke auch
            if(fav->GetBotData(BOT_Power, favmode) >= BOT_Power_3)
              break;
    }
  }
  // Auswählen
  if(!fav) return;
  // Feuermodus wechseln?
  if(fFireModes)
    if(favmode && favmode != fav->GetFireMode())
      fav->SetFireMode(favmode);
  if(ContentsCount() == 1) return(1);
  return(ShiftContents(0,0,fav->GetID()));
}

protected func MacroComMoveTo()
{
	var x, y;
	x = GetMacroCommand(0,2);
	y = GetMacroCommand(0,3);
	if(!inherited()) return;
	SetCommand(this, "MoveTo", 0, x, y);
	return;
}
