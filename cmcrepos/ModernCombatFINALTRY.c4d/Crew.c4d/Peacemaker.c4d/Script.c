/*--- Der Peacemaker ---*/

#strict 2
#include HZCK

static const PCMK_PortraitVersion = 130;

public func WeaponCollectionLimit() {	return 3; } //Der Clonk kann drei Waffen tragen
public func ObjectCollectionLimit() { return 2; } //Und 2 beliebige Zusatzobjekte


/* Initialisierung */

public func Initialize()
{
	UpdateAmmoBars();
	return _inherited();
}

/* Portrait-Updates */

protected func Recruitment()
{
	if (GetCrewExtraData(this, "CMC_Portrait") < PCMK_PortraitVersion)
	{
		SetCrewExtraData(this, "CMC_Portrait", PCMK_PortraitVersion);
		SetPortrait("random", this, GetID(), true, true);
	}
	return _inherited(...);
}

public func CanUse(id idObj)
{
	if (idObj == HARM)
		return;
	return _inherited(idObj, ...);
}

private func AbortWalk()
{
	if (GetAction() == "Dive")
	{
		var c = Contents();
		if (c)
		{
			if ((c->~IsWeapon() && !c->~GetFMData(FM_Aim)) || c->~IsGrenade())
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
	if (FindObject(NOAM))
	{
		RemoveAll(ABAR);
	}
	else 
	{
		for (var clonk in FindObjects(Find_OCF(OCF_Alive), Find_OCF(OCF_CrewMember), Find_NoContainer(), Find_Exclude(this))) 
		{
			var breaks = false;
			for (var bar in FindObjects(Find_ID(ABAR), Find_ActionTarget(clonk))) 
			{
				if (GetOwner(bar) == GetOwner())
				{
					if (!Contents() || GetID(Contents()) != AMPK)
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
			if (breaks)
				continue;
			if (!Contents() || GetID(Contents()) != AMPK)
				continue;
			if (Hostile(GetOwner(clonk), GetOwner()))
				continue;
			if (!GetPlayerName(GetController(clonk)))
				continue;
			CreateObject(ABAR, AbsX(GetX(clonk)), AbsY(GetY(clonk)), GetOwner())->Set(clonk, RGB(255, 255, 80), GetLength(FindObjects(Find_Func("IsBar"), Find_Func("BarActive"), Find_ActionTarget(clonk), Find_Owner(GetOwner()))) + 1);
		}
	}
	ScheduleCall(0, "UpdateAmmoBars", 1);
}

/* KI-Erweiterung */

public func GetReanimationTarget(pFrom, & body, & defi, fUnderAttack)
{
	var olddefi;
	var distance = 200;
	if (fUnderAttack)
		distance = 50;
	body = FindObject2(Find_Func("IsFakeDeath"), Find_Category(C4D_Living), Find_Distance(distance, AbsX(GetX(pFrom)), AbsY(GetY(pFrom))), Find_Allied(GetOwner(pFrom)), Sort_Distance(AbsX(GetX(pFrom)), AbsY(GetY(pFrom))));
	if (body)
	{
		if (ContentsCount(CDBT, pFrom))
		{
			defi = FindObject2(Find_ID(CDBT), Find_Container(pFrom));
			if (!defi->~Ready())
			{
				olddefi = defi;
			}
		}
		if (!defi)
			defi = FindObject2(Find_ID(CDBT), Find_Or(Find_Container(Contained(body)), Find_Container(pFrom), Find_NoContainer()), Find_Func("Ready"), Find_Distance(distance, AbsX(GetX(pFrom)), AbsY(GetY(pFrom))), Sort_Distance(AbsX(GetX(pFrom)), AbsY(GetY(pFrom))));
		if (defi)
		{
			if(olddefi) {
				pFrom->~DropObject(olddefi);
			}
			return true;
		}
	}
	body = false;
	return false;
}

public func FxAggroTimer(object pTarget, int no)
{
	if (Contained())
		return;
	// Hilfsbedürftige in der Nähe?
	var body, defi;
	GetReanimationTarget(pTarget, body, defi, EffectVar(1, this, no)); //Checkt auch nach Defi
	if (body)
	{
		if (IsAiming())
			StopAiming();
		if (Contents() == defi && GetProcedure(pTarget) && ObjectDistance(body, pTarget) < 10)
		{
      defi->Activate(this);
			return;
		}
		else 
		{
			if (!Contained(defi) || Contained(defi) != pTarget)
			{
				if (GetCommand(pTarget) != "Get")
					SetCommand(pTarget, "Get", defi);
			}
			else if (Contents() != defi)
			{
				ShiftContents(pTarget, 0, CDBT, true);
			}
			else 
			{
				SetMacroCommand(pTarget, "MoveTo", body, 0, 0, 0, EffectVar(0, pTarget, no));
			}
			return 1;
		}
	}
	// Verletzt?
	if (!pTarget->~IsHealing() && pTarget->GetEnergy() < pTarget->GetPhysical("Energy") * 2 / 3 / 1000)
	{
		if (!ContentsCount(DGNN, pTarget) && ContentsCount(FAPK, pTarget))
		{
			var pFAP = FindObject2(Find_ID(FAPK), Find_Container(pTarget), Find_Func("CanUnpack", pTarget));
			if (pFAP)
				pFAP->ControlThrow(pTarget);
		}
		var pDragnin = FindObject2(Find_ID(DGNN), Find_Container(pTarget));
		if (pDragnin)
			pDragnin->Activate(pTarget);
	}
	// Weitere Verletzte?
	if(pTarget->~IsMedic()) {
		for (var friend in FindObjects(Find_Category(OCF_Living), Find_Allied(GetOwner(pTarget)), Find_NoContainer(), Find_Distance(50, AbsX(GetX(pTarget)), AbsY(GetY(pTarget))), Sort_Distance(AbsX(GetX(pTarget)), AbsY(GetY(pTarget))))) 
		{
			if (!friend->~IsHealing() && friend->GetEnergy() < friend->GetPhysical("Energy") * 1 / 3 / 1000)
			{
				if (!ContentsCount(DGNN, pTarget) && ContentsCount(FAPK, pTarget))
				{
					var pFAP = FindObject2(Find_ID(FAPK), Find_Container(pTarget), Find_Func("CanUnpack", pTarget));
					if (pFAP)
            pDragnin->ControlThrow(pTarget);
				}
				var pDragnin = FindObject2(Find_ID(DGNN), Find_Container(pTarget));
				if (pDragnin)
				{
					if (ObjectDistance(friend, pTarget) < 10)
					{
						pDragnin->ControlThrow(pTarget);
					}
					else 
					{
						SetMacroCommand(pTarget, "MoveTo", friend, 0, 0, 0, EffectVar(0, pTarget, no));
					}
				}
				break;
			}
		}
	}
	// Ziel vorhanden?
  if(EffectVar(1, this, no)) { EffectCall(this, no, "Fire"); return true; }
	// Zielen beenden
	if (IsAiming())
		StopAiming();
	// Ziel suchen
	var dir = GetDir() * 2 - 1;
	// Vorne
	var target = GetTarget(90 * dir, 90);
	// Hinten
	if (!target)
		if ((!GetCommand() && !GetMacroCommand()) || EffectVar(0, this, no) != 1)
			target = GetTarget(-90 * dir, 90);
	// Gefunden?
	if (!target)
	{
		if (EffectVar(99, this, no))
		{
			if (Contained())
				Contained()->~HandleAggroFinished(this);
			else if (IsRiding())
				GetActionTarget()->~HandleAggroFinished(this);
			EffectVar(99, this, no);
		}
		// Kein Ziel gefunden, also andere Sachen tun
		CheckIdleInventory();
		CheckIdleWeapon();
		return;
	}
	EffectVar(1, this, no) = target;
	// Ziel vorhanden
	EffectVar(99, this, no) = true;
}

//Wie haben nichts zu tun und spielen mit dem Inventar rum
public func CheckIdleInventory()
{
  for(var i=0,obj ; obj = Contents(i) ; i++)
  {
  	// Irgendwas spezielles?
    if(obj->~AI_IdleInventory(this))
      continue;
    // Waffe
    if(obj->~IsWeapon())
      continue;
    // Munition
    if(obj->~IsAmmoPacket())
    {
      ActivateAmmo(obj);
      continue;
    }
    //Nichts weiter tun
  }
}

public func FxAggroFire(object pTarget, int no)
{
  // Zusatzhack: BR-Bombe!
  if(GetID(Contents()) == GBRB)
    // Nichts tun :C
    return;
  if(Contents() && Contents()->~RejectShift())
    return;
  // Nichts tun, wenn gerade verhindert
  if(!ReadyToFire()) return;
  var y = EffectVar(4, this, no);
  var x = EffectVar(3, this, no);
  var dist = EffectVar(2, this, no);
  var target = EffectVar(1, this, no);
  var level = EffectVar(0, this, no);
  var pathfree = true;
  
  // Fahrzeugsteuerung
  if(Contained())
  {
    if(Contained()->~HandleAggro(this, level, target, dist, x, y))
      return(1);
    else
      return(AddCommand(this, "Exit", 0,0,0,0,0,0,0, C4CMD_SilentSub));
  }
  if(IsRiding())
  {
    if(GetActionTarget()->~HandleAggro(this, level, target, dist, x, y))
      return(1);
    else
      return(SetAction("Walk"));
  }
  
  // Zu weit von der Wachposition entfernt?
  if(level == 3) {
    if(Distance(GetX(), GetY(), x, y) > dist)
    {
      if(GetMacroCommand(1, 1) == target)
      {
        FinishMacroCommand(1,0,1);
        FinishMacroCommand(1);
      }
      AddMacroCommand(0, "MoveTo", 0, x,y, 0, level);
      EffectVar(1, this, no) = 0;
      return;
    }
  }
  
  var maxdist = dist;
  if(!PathFree(GetX(), GetY(), target->GetX(), target->GetY()))
  {
    if(level == 1) maxdist = 0;
    if(level >= 2) maxdist = dist/2;
    pathfree = false;
    target = 0;
  }
  
  // Ziel irgendwie weg?
  // (Pathfree wurde schon gecheckt)
  if(!CheckTarget(target,this,maxdist,0,0,true))
    {
      EffectVar(1, this, no) = 0;
      if(EffectVar(0, this, no) == 2)
        ClearMacroCommands();
      if(IsAiming())
        StopAiming();
      return;
    }
  
  /* Verziehen wir zu stark?
  //Log("%d/%d", GetSpread(), ObjectDistance(target));
  if(Contents()->~IsWeapon2() && GetSpread() > 100) return;//BoundBy(300 - ObjectDistance(target), 80, 280)) return;*/
  
  // Ich hab nix? °-°
  if(!Contents()) return; // Lauf, Forest, lauf!
  // Waffe in die Hand nehmen
  if(!SelectWeapon(level, target, false))
    // Evtl. Feuermodus wechseln (dann muss erst nachgeladen werden, aber besser als nichts)
    if(!SelectWeapon(level, target, true))
    {
      // Bei Aggro_Follow können wir von unserem Pfade weg. D.h. eine Waffe und/oder Munition muss her
      if(GetAggroLevel() == Aggro_Follow)
      {
//      Message("@Searching for weapons / ammo", this);
        // Waffen auffrischen?
        if(CustomContentsCount("IsWeapon") <= 1)
          return(SearchWeapon(Aggro_Shoot));
        // Munition auffrischen
        return(SearchAmmo(Aggro_Shoot));
      }
      // ein Balrog, ein Feind gegen den ihr nichts ausrichten könnt...lauft!
      return;
    }
  // Stufe 1 - nur in die grobe Richtung ballern, lieber nicht anhalten oder sowas

  // Schaue ich in die richtige Richtung?
  if(GetX() < target->GetX())
  {
    if(GetDir() != DIR_Right)
      SetDir(DIR_Right);
  }
  else
  {
    if(GetDir() != DIR_Left)
      SetDir(DIR_Left);
  }

  // Zielen, muss auch mal sein
 if(((!GetCommand() && !GetMacroCommand()) || level != 1 || IsAiming()) && ReadyToAim())
 {
  if(pathfree && Contents()->GetBotData(BOT_Range) > 30) // Weg frei und keine Nahkampfwaffe?
  {
    var angle1 = Angle(GetX(), GetY(), GetX(target), GetY(target)+GetDefHeight(GetID(target))/2)-GetSpread()/50;
    var angle2 = Angle(GetX(), GetY(), GetX(target), GetY(target)-GetDefHeight(GetID(target))/2)+GetSpread()/50;
    if(Contents()->GetBotData(BOT_Ballistic) || ((!Inside(90, angle1, angle2) && !Inside(270, angle1, angle2)) || Contents()->GetFMData(FM_Aim) > 0))
    {
      if(!IsAiming()) StartSquatAiming();
      if(IsAiming())
      {
        var tx = target->GetX();
        var ty = target->GetY();
    
        if(Contents()->GetBotData(BOT_Ballistic))
          ty -= 25;
    
        DoMouseAiming(tx, ty);
      }
    }
    else
      if(IsAiming())
        StopAiming();
  }
  if(IsAiming() && !CheckAmmo(Contents()->GetFMData(FM_AmmoID), Contents()->GetFMData(FM_AmmoLoad), Contents(), this))
    StopAiming();
 }

   // Gut. Feuern wir bereits?
  if(Contents()->IsRecharging() || Contents()->IsShooting()) return;

  // Feuer!
  if(maxdist != 300 && pathfree) {
  	Control2Contents("ControlThrow");
  }
  else {
      if(IsAiming())
        StopAiming();
  }
//  Message("@My target: %s @%d/%d with level %d", this, target->GetName(), target->GetX(), target->GetY(), level);
  // Stufe 2 - verfolgen!
  /*if(EffectVar(0, this, no) >= 2 && dist > 0)
    if(GetMacroCommand(1) != "Follow" || GetMacroCommand(1, 1) != target)
      if(GetMacroCommand(0) != "Follow" || GetMacroCommand(0,1) != target)
      {
        DebugLog("FxAggroFire - Adding Follow command","aggro");
        AddMacroCommand(0, "MoveTo", 0, GetX(),GetY(), 0, level);
        AddMacroCommand(0, "Follow", target, 0, 0, 0, level);
      }*/
}

//Wenn iLevel = 1 (Aggro_Shoot) werden keine Waffen mit FM_Aim ausgewählt
public func SelectWeapon(int iLevel, object pTarget, bool fFireModes)
{
	//Entfernung zum Ziel
	var dist = ObjectDistance(pTarget);
	//Keine Waffen in Inventar?
	if (!CustomContentsCount("IsWeapon"))
		return;
	//Bevorzugten Schadenstyp bestimmen
	var preftype = GetPrefDmgType(pTarget), type;
	//Alle durchgehen und passende prüfen
	for (var i = 0, obj, fav, mode, favmode; obj = Contents(i); mode++)
	{
		if (!(obj->~IsWeapon()))
		{
			i++;
			mode = -1;
			continue;
		}
		if (mode && !fFireModes)
		{
			i++;
			mode = -1;
			continue;
		}
		if (!(obj->GetFMData(FM_Name, mode)))
		{
			i++;
			mode = -1;
			continue;
		}
		if (mode == obj->GetFireMode() && mode)
			continue;
		if (obj->GetFMData(FM_Aim, mode) > 0)
			if (iLevel == 1 || !WildcardMatch(GetAction(), "*Walk*"))
				continue;
		if (!NoAmmo() && !(obj->GetCharge()) && !GetAmmo(obj->GetFMData(FM_AmmoID, mode)))
			continue;
		if (obj->GetBotData(BOT_EMP, mode))
			if (!(pTarget->~IsMachine()))
				continue;
		if (!fav)
		{
			fav = obj;
			type = fav->GetBotData(BOT_DmgType, mode);
			favmode = mode;
		}
		else 
		{
			if (fav->GetBotData(BOT_Range, favmode) < dist)
			{
				if (obj->GetBotData(BOT_Range, mode) > dist)
				{
					fav = obj;
					type = obj->GetBotData(BOT_DmgType, mode);
					favmode = mode;
				}
			}
			else 
			{
				if (pTarget->~OnDmg(obj->GetBotData(BOT_DmgType, mode)) < pTarget->~OnDmg(type) && fav->GetBotData(BOT_Power, favmode) - 1 <= obj->GetBotData(BOT_Power, mode))
				{
					fav = obj;
					type = fav->GetBotData(BOT_DmgType);
					favmode = mode;
				}
				else 
				{
					if (fav->GetBotData(BOT_Power, favmode) < obj->GetBotData(BOT_Power, mode) || (fav->GetBotData(BOT_Power, favmode) == BOT_Power_LongLoad && (fav->IsReloading() || !(fav->GetCharge()))))
					{
						if (obj->GetBotData(BOT_Power, mode) != BOT_Power_LongLoad)
						{
							fav = obj;
							type = fav->GetBotData(BOT_DmgType);
							favmode = mode;
						}
						else if (obj->GetCharge() != 0 && !(obj->IsReloading()))
						{
							fav = obj;
							type = fav->GetBotData(BOT_DmgType);
							favmode = mode;
						}
					}
				}
			}
			if (fav->GetBotData(BOT_Range, favmode) >= dist)
				if (preftype == type)
					if (fav->GetBotData(BOT_Power, favmode) >= BOT_Power_3)
						break;
		}
	}
	//Auswählen
	if (!fav)
		return;
	//Feuermodus wechseln?
	if (fFireModes)
		if (favmode && favmode != fav->GetFireMode())
			fav->SetFireMode(favmode);
	if (ContentsCount() == 1)
		return 1;
	ShiftContents(0, 0, fav->GetID(), true);
	return true;
}

/* Waffenbehandlung wenn nicht im Kampf */

public func CheckIdleWeapon()
{
  if(Contents()) {
    // Hack - mit BR-Bombe tut er gar nichts
    if(Contents()->GetID() == GBRB) return;
    if(Contents()->~RejectShift()) return;
  }
  // Keine Waffen im Inventar
  if(!CustomContentsCount("IsWeapon")) return;
  // nachladende Waffe in der Hand
  if(Contents()->~IsWeapon())
    if(Contents()->IsReloading() || Contents()->~IsRecharging())
      return;
  // Inventar nach Waffe durchsuchen, die man Nachladen könnte
  for(var i=0, mode=1, obj; obj = Contents(i) ; mode++)
  {
    // Keine Waffe
    if(!(obj->~IsWeapon()))
    {
      i++;
      mode = 0;
      continue;
    }
    // Waffe hat gar nicht so viele Modi
    if(!(obj->GetFMData(FM_Name, mode)))
    {
      i++;
      mode = 0;
      continue;
    }
    // Waffe ist voll geladen
    if(obj->GetAmmo(obj->GetFMData(FM_AmmoID, mode)) >= obj->GetFMData(FM_AmmoLoad, mode) / 2)
    {
      i++;
      mode = 0;
      continue;
    }
    // EMP-Modi erstmal nicht laden
    if(obj->GetBotData(BOT_EMP, mode)) continue;
    // Waffe ist nachladbar
    if(CheckAmmo(obj->GetFMData(FM_AmmoID, mode), obj->GetFMData(FM_AmmoLoad, mode) - obj->GetAmmo(obj->GetFMData(FM_AmmoID, mode))))
    {
      //mode = obj->GetFireMode();
      break;
    }
    // Nächsten Feuermodus prüfen
    if(mode == obj->GetFireMode()) continue;
    if(CheckAmmo(obj->GetFMData(FM_AmmoID, mode), obj->GetFMData(FM_AmmoLoad, mode)))
      break;
  }
  // Nix gefunden
  if(!Contents(i)) return;
  
  // Aha! Waffe wechseln!
  if(ContentsCount() != 1 && Contents() != obj)
    SelectInventory(obj);
  // Feuermodus wechseln
  if(obj->GetFireMode() != mode)
    obj->SetFireMode(mode);
  // Und Muni reinhauen
  if(!Contents()->~IsRecharging())
    Schedule("Control2Contents(\"ControlThrow\")", 1);
  // Klasse
  return(1);
}

protected func MacroComMoveTo()
{
	var x, y;
	x = GetMacroCommand(0, 2);
	y = GetMacroCommand(0, 3);
	if (!inherited())
		return;
	SetCommand(this, "MoveTo", 0, x, y);
	return;
}
