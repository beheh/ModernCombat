/*--- Der Peacemaker ---*/

#strict 2
#include HZCK

static const PCMK_PortraitVersion = 160;

public func WeaponCollectionLimit()	{return 3;}				//Anzahl Waffen im Inventar
public func ObjectCollectionLimit()	{return 2;}				//Anzahl Objekte im Inventar
public func DeathAnimationCount()	{return (GetID() == PCMK) && 6;}	//Anzahl Todesanimationen


/* Portrait-Updates */

protected func Recruitment()
{
  if(GetID() == PCMK)
  {
    if(GetCrewExtraData(this, "CMC_Portrait") < PCMK_PortraitVersion)
    {
      SetCrewExtraData(this, "CMC_Portrait", PCMK_PortraitVersion);
      SetPortrait("random", this, GetID(), true, true);
    }
  }
  return _inherited(...);
}

/* Aktionen */

public func CanUse(id idObj)
{
  if(idObj == HARM)
    return;
  return _inherited(idObj, ...);
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

/* Abseilen vom Helikopter */

protected func FxCheckGroundStart(object pTarget, int iNo, int iTemp, object pHeli)
{
  if(!pHeli)
    return;

  //Seil erstellen und verbinden
  var pRope = CreateObject(CK5P, 0, 0, GetOwner(pTarget));
  pRope->ConnectObjects(pHeli ,pTarget);
  pRope->SetRopeLength(10);

  //Effekte
  Sound("ZiplineIn.ogg", 0, pTarget, 40);
  Sound("ZiplineSlide.ogg", 0, pTarget, 40, 0, +1);

  EffectVar(0, pTarget, iNo) = pRope;	//Das Seil
  EffectVar(1, pTarget, iNo) = pHeli;	//Der Helikopter
}

protected func FxCheckGroundTimer(object pTarget, int iNo, int iTime)
{
  var pRope = EffectVar(0, pTarget, iNo);
  var pHeli = EffectVar(1, pTarget, iNo);
  
  //Heli oder Seil weg, Knapp über dem Boden, falsche Aktion oder Seil zu lang?
  if(!pHeli || !pRope
     || !PathFree(GetX(pTarget), GetY(pTarget), GetX(pTarget), GetY(pTarget) + 30)
     || pRope->GetRopeLength() > 1000
     || !WildcardMatch(GetAction(pTarget), "*Jump*"))
  {
    //eventuell noch vorhandenes Seil entfernen und Absprung verlangsamen
    if(pRope)
      RemoveObject(pRope);
    SetYDir(20,pTarget);

    return -1;
  }
  else
    pRope->SetRopeLength(iTime * 4 + 10);
}

protected func FxCheckGroundStop(object pTarget, int iEffectNumber)
{
  //Effekte
  Sound("ZiplineOut.ogg", 0, pTarget, 40);
  Sound("ZiplineSlide.ogg", 0, pTarget, 40, 0, -1);
  return;
}

/* Automatisches Defibrillator-Auslösen */

public func FxIntActivatingShockPaddlesStart(object pTarget, int iEffectNumber, bool fTemp, object pShockPaddlesOwner, object pStartItem)
{
  if(fTemp)
    return true;

  EffectVar(0, pTarget, iEffectNumber) = pShockPaddlesOwner;
  EffectVar(1, pTarget, iEffectNumber) = pStartItem;

  return true;
}

public func FxIntActivatingShockPaddlesTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(iEffectTime >= 20)
  {
    Contents(0, this)->~Activate(this);
    var pShockPaddlesOwner = EffectVar(0, pTarget, iEffectNumber);
    if(pShockPaddlesOwner && pShockPaddlesOwner != this)
      if(!pShockPaddlesOwner->~RejectCollect(GetID(Contents(0, this)), Contents(0, this)))
      {
        Enter(pShockPaddlesOwner, Contents(0, this));

        //Items ggf. wieder zurueckrotieren
        if(EffectVar(1, pTarget, iEffectNumber) && FindObject2(Find_Container(this), Find_Not(Find_Exclude(EffectVar(1, pTarget, iEffectNumber)))))
          while(Contents(0, this) != EffectVar(1, pTarget, iEffectNumber))
            ShiftContents(this);
      }

    return -1;
  }

  return 1;
}

public func ActivateShockPaddles(object pShockPaddlesOwner)
{
  //Abbruch bei keinem Defibrillator
  if(!FindObject2(Find_Func("IsShockPaddles"), Find_Container(this)))
    return;

  //Defibrillator wenn nötig anwählen
  if(!Contents(0, this)->~IsShockPaddles())
  {
    var pStartItem = Contents(0, this);
    while(!Contents(0, this)->~IsShockPaddles())
      ShiftContents(this);

    //Verzögertes Auslösen des Defibrillators um Auswahlzeiten einzuhalten
    AddEffect("IntActivatingShockPaddles", this, 100, 20, this, GetID(this), pShockPaddlesOwner, pStartItem);
  }
  else
    //Ansonsten sofort auslösen
    if(!GetEffect("IntActivatingShockPaddles"))
    {
      Contents(0, this)->~Activate(this);
      //Ggf. wieder zuruecklegen
      if(pShockPaddlesOwner && pShockPaddlesOwner != this)
        if(!pShockPaddlesOwner->~RejectCollect(GetID(Contents(0, this)), Contents(0, this)))
          Enter(pShockPaddlesOwner, Contents(0, this));
    }

  return 1;
}

/* KI-Erweiterung */

/* Schwerverletzte suchen */

public func GetReanimationTarget(pFrom, & body, & defi, fUnderAttack)
{
  var olddefi, distance = 200;
  if(fUnderAttack)
    distance = 50;
  body = FindObject2(Find_Func("IsFakeDeath"), Find_Category(C4D_Living), Find_Distance(distance, AbsX(GetX(pFrom)), AbsY(GetY(pFrom))), Find_Allied(GetOwner(pFrom)), Sort_Distance(AbsX(GetX(pFrom)), AbsY(GetY(pFrom))));
  if(body && Contained(body) && !(Contained(body)->~RejectReanimation()))
  {
    //Defibrillator zum Reanimieren suchen
    if(ContentsCount(CDBT, pFrom))
    {
      defi = FindObject2(Find_Func("IsShockPaddles"), Find_Container(pFrom));
      if(!defi->~Ready())
      {
        olddefi = defi;
      }
    }
    if(!defi)
      defi = FindObject2(Find_Func("IsShockPaddles"), Find_Or(Find_Container(Contained(body)), Find_Container(pFrom), Find_NoContainer()), Find_Func("Ready"), Find_Distance(distance, AbsX(GetX(pFrom)), AbsY(GetY(pFrom))), Sort_Distance(AbsX(GetX(pFrom)), AbsY(GetY(pFrom))));
    if(defi)
    {
      if(olddefi)
      {
        pFrom->~DropObject(olddefi);
      }
      return true;
    }
  }
  body = false;
  return false;
}

/* Allgemeine Aufgabensuche */

public func FxAggroTimer(object pTarget, int no)
{
  //Abbrechen wenn verschachtelt
  if(Contained())
    return;

  //Keine Aktionen wenn geblendet
  var srgb;
  if((srgb = GetScreenRGB(GetOwner(), SR4K_LayerLight)) || (srgb = GetScreenRGB(GetOwner(), SR4K_LayerSmoke)))
  {
    var a = srgb->GetAlpha();
    if(a < 200)
    {
      if(Contents()->~IsWeapon() && Contents()->~IsShooting())
        Contents()->StopAutoFire();

      return;
    }
  }

  //Hilfsbedürftige in der Nähe?
  var body, defi;
  if(GetPlayerViewAlpha(GetOwner()) > 0)
  {
    //Schwerverletzte suchen
    GetReanimationTarget(pTarget, body, defi, EffectVar(1, this, no));
    if(body)
    {
      //Eventuelles Zielen einstellen
      if(IsAiming())
        StopAiming();
      if(Contents() == defi && GetProcedure(pTarget) && ObjectDistance(body, pTarget) < 10)
      {
        defi->Activate(this);
        return;
      }
      else
      {
        if(!Contained(defi) || Contained(defi) != pTarget)
        {
          if(GetCommand(pTarget) != "Get")
            SetCommand(pTarget, "Get", defi);
        }
        else
        if(!Contents()->~IsShockPaddles())
        {
          while(!Contents()->~IsShockPaddles())
            ShiftContents(pTarget);
        }
        else 
        {
          SetMacroCommand(pTarget, "MoveTo", body, 0, 0, 0, EffectVar(0, pTarget, no));
        }
        return 1;
      }
    }
    //Verletzte suchen
    if(!pTarget->~IsHealing() && pTarget->GetEnergy() < pTarget->GetPhysical("Energy") * 2 / 3 / 1000)
    {
      //EHP oder Dragnin suchen und einsetzen
      if(!ContentsCount(DGNN, pTarget) && ContentsCount(FAPK, pTarget))
      {
        var pFAP = FindObject2(Find_ID(FAPK), Find_Container(pTarget), Find_Func("CanUnpack", pTarget));
        if(pFAP)
          pFAP->ControlThrow(pTarget);
      }
      var pDragnin = FindObject2(Find_ID(DGNN), Find_Container(pTarget));
      if(pDragnin)
        pDragnin->Activate(pTarget);
    }
    //Weitere Verletzte suchen
    if(pTarget->~IsMedic())
    {
      for (var friend in FindObjects(Find_OCF(OCF_Alive), Find_Exclude(pTarget), Find_Allied(GetOwner(pTarget)), Find_NoContainer(), Find_Distance(100, AbsX(GetX(pTarget)), AbsY(GetY(pTarget))), Sort_Distance(AbsX(GetX(pTarget)), AbsY(GetY(pTarget))))) 
      {
        if(!friend->~IsHealing() && (friend->GetEnergy() < friend->GetPhysical("Energy") * 2 / 3 / 1000 ))
        {
          //EHP oder Dragnin suchen und einsetzen
          if(!ContentsCount(DGNN, pTarget) && ContentsCount(FAPK, pTarget))
          {
            var pFAP = FindObject2(Find_ID(FAPK), Find_Container(pTarget), Find_Func("CanUnpack", pTarget));
            ShiftContents(pTarget, 0, FAPK, true);
            if(pFAP)
              pDragnin->ControlThrow(pTarget);
          }
          var pDragnin = FindObject2(Find_ID(DGNN), Find_Container(pTarget));
          if(pDragnin)
          {
            if(ObjectDistance(friend, pTarget) < 10)
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
  }
  //Ziel vorhanden?
  if(EffectVar(1, this, no))
  {
    EffectCall(this, no, "Fire"); return true;
  }
  //Zielen beenden
  if(IsAiming())
    StopAiming();
  //Ziel suchen
  var dir = GetDir() * 2 - 1;
  //Vorne
  var target = GetTarget(90 * dir, 90);
  //Hinten
  if(!target)
    if((!GetCommand() && !GetMacroCommand()) || EffectVar(0, this, no) != 1)
      target = GetTarget(-90 * dir, 90);
  //Gefunden?
  if(!target)
  {
    if(EffectVar(99, this, no))
    {
      if(Contained())
        Contained()->~HandleAggroFinished(this);
      else if(IsRiding())
        GetActionTarget()->~HandleAggroFinished(this);
      EffectVar(99, this, no);
    }
    //Kein Ziel gefunden: Andere Aufgaben suchen
    if(!Contents()->~RejectShift())
    {
      CheckIdleInventory();
      CheckIdleWeapon();
    }
    return;
  }
  EffectVar(1, this, no) = target;
  //Ziel vorhanden
  EffectVar(99, this, no) = true;
}

/* Inventar-Aktionen bei Inaktivität */

public func CheckIdleInventory()
{
  for(var i=0,obj ; obj = Contents(i) ; i++)
  {
    //Irgendwas spezielles?
    var result = obj->~AI_IdleInventory(this);
    if(result)
    {
      if(result > 1)
        return result;
      continue;
    }
    //Waffe
    if(obj->~IsWeapon())
      continue;
    //Munition
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
  //Bei BR-Bombe nichts tun
  if(GetID(Contents()) == GBRB)
    return;
  if(Contents() && Contents()->~RejectShift())
    return;
  //Nichts tun wenn gerade verhindert
  if(!ReadyToFire() && !(Contents()->~IsWeapon() && Contents()->GetFMData(FM_Aim) && ReadyToAim()))
    return;
  var y = EffectVar(4, this, no);
  var x = EffectVar(3, this, no);
  var dist = EffectVar(2, this, no);
  var target = EffectVar(1, this, no);
  var level = EffectVar(0, this, no);
  var pathfree = true;

  //Fahrzeugsteuerung
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

  //Zu weit von der Wachposition entfernt?
  if(level == 3)
  {
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
  if(!Contents()->~AI_IgnorePathFree(this, target) && !PathFree(GetX(), GetY(), target->GetX(), target->GetY()))
  {
    if(level == 1) maxdist = 0;
    if(level >= 2) maxdist = dist/2;
    pathfree = false;
    target = 0;
  }

  //Ziel verschwunden?
  if(!CheckTarget(target,this,maxdist,0,0,true))
  {
    Contents()->~AI_TargetLost(this, target, maxdist);
    EffectVar(1, this, no) = 0;
    if(EffectVar(0, this, no) == 2)
      ClearMacroCommands();
    if(IsAiming())
      StopAiming();
    return;
  }

  /*
  //Verziehen wir zu stark?
  //Log("%d/%d", GetSpread(), ObjectDistance(target));
  if(Contents()->~IsWeapon2() && GetSpread() > 100) return;//BoundBy(300 - ObjectDistance(target), 80, 280)) return;
  */

  //Keine Ausrüstung?
  if(!Contents())
    return;
  //Waffe in die Hand nehmen
  if(!SelectWeapon(level, target, false))
  {
    //Eventuell Feuermodus wechseln
    if(!SelectWeapon(level, target, true))
    {
      //Nach Ausrüstungsgegenstand suchen
      if(SelectWeapon(level, target, false, false, true))
      {
        //Botsteuerung dem Ausrüstungsgegenstand überlassen.
        return Contents()->~BotControl(this, target, level, true);
      }
      //Ansonsten, falls möglich, leere Waffe mit langer Nachladezeit nachladen.
      else if(!SelectWeapon(level, target, true, true))
      {
        //Bei Aggro_Follow Waffe und/oder Munition besorgen
        if(GetAggroLevel() == Aggro_Follow)
        {
          //Message("@Searching for weapons / ammo", this);
          //Waffen auffrischen?
          if(CustomContentsCount("IsWeapon") <= 1)
            return(SearchWeapon(Aggro_Shoot));
          //Munition auffrischen
          return(SearchAmmo(Aggro_Shoot));
        }
        return;
      }
      //Wir haben nur leere Waffen zur Verfügung die eine lange Nachladezeit haben -> geeignetste Waffe nachladen
      else
        if(Contents()->GetBotData(BOT_Power) == BOT_Power_LongLoad && !(Contents()->GetCharge()))
          Contents()->~Reload();
    }
  }

  // Stufe 1 - nur in die grobe Richtung feuern

  //Clonk entsprechend der Zielposition drehen
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

  var meleeattack;
  var fNeedAim = Contents()->~AI_NeedAim(this, target);
  if(!Contents()->GetFMData(FM_Aim) && !fNeedAim && (meleeattack = FindObject2(Find_Not(Find_Exclude(target)), Find_AtPoint())))
  {
    if(IsAiming())
      StopAiming();
  }
  //Ziel anvisieren
  else
    if((((!GetCommand() && !GetMacroCommand()) || level != 1) && ReadyToAim()) || IsAiming())
    {
      if(pathfree && Contents()->GetBotData(BOT_Range) > 30) // Weg frei und keine Nahkampfwaffe?
      {
        var angle1 = Angle(GetX(), GetY(), GetX(target), GetY(target)+GetDefHeight(GetID(target))/2)-GetSpread()/50;
        var angle2 = Angle(GetX(), GetY(), GetX(target), GetY(target)-GetDefHeight(GetID(target))/2)+GetSpread()/50;
        if(Contents()->GetBotData(BOT_Ballistic) || ((!Inside(90, angle1, angle2) && !Inside(270, angle1, angle2)) || Contents()->GetFMData(FM_Aim) > 0))
        {
          if(!IsAiming()) StartSquatAiming();
          if(IsAiming() && !Contents()->~AI_NeedControl(this, target))
          {
            var tx = target->GetX();
            var ty = target->GetY();

            if(Contents()->GetBotData(BOT_Ballistic))
              ty -= 25;

            DoMouseAiming(tx, ty);
          }
        }
        else
          if(IsAiming() && !fNeedAim)
            StopAiming();
      }
      if(IsAiming() && !CheckAmmo(Contents()->GetFMData(FM_AmmoID), Contents()->GetFMData(FM_AmmoLoad), Contents(), this) && !fNeedAim)
        StopAiming();
    }

  //Bereits am Feuern?
  if(Contents()->IsRecharging() || Contents()->IsShooting())
    return;

  //Feuer frei
  if(maxdist != 300 && pathfree && !(Contents()->~IsReloading()))
  {
    if(meleeattack || AngleOffset4K(Angle(GetX(), GetY(), GetX(target), GetY(target)), Contents()->~AimAngle()) <= Contents()->~GetBotData(BOT_Precision))
      Control2Contents("ControlThrow");
  }
  else if(IsAiming() && !fNeedAim)
    StopAiming();
  //Message("@My target: %s @%d/%d with level %d", this, target->GetName(), target->GetX(), target->GetY(), level);

  //Stufe 2 - Ziel verfolgen

  /*
  if(EffectVar(0, this, no) >= 2 && dist > 0)
  if(GetMacroCommand(1) != "Follow" || GetMacroCommand(1, 1) != target)
    if(GetMacroCommand(0) != "Follow" || GetMacroCommand(0,1) != target)
    {
      DebugLog("FxAggroFire - Adding Follow command","aggro");
      AddMacroCommand(0, "MoveTo", 0, GetX(),GetY(), 0, level);
      AddMacroCommand(0, "Follow", target, 0, 0, 0, level);
    }
  */
}

/* Waffenanwahl */

local favprio, favprioweapon;

//Wenn iLevel = 1 (Aggro_Shoot) werden keine Waffen mit FM_Aim ausgewählt

public func SelectWeapon(int iLevel, object pTarget, bool fFireModes, bool fEmptyLongLoad, bool fEquipment)
{
  //Entfernung zum Ziel
  var dist = ObjectDistance(pTarget);
  //Keine Waffen in Inventar?
  if(!fEquipment && !CustomContentsCount("IsWeapon"))
    return;
  //Die aktuelle Waffe muss kontrolliert werden?
  if(Contents()->~AI_NeedControl())
    return true;
  //Bevorzugten Schadenstyp bestimmen
  var preftype = GetPrefDmgType(pTarget), type;
  //Alle durchgehen und passende prüfen
  for (var i = 0, obj, fav, mode, favmode; obj = Contents(i); mode++)
  {
    //Ausrüstungsgegenstand unterstützt die KI?
    if(fEquipment && obj->~IsEquipment() && obj->~HasBotSupport(this, pTarget))
    {
      fFireModes = false;
      fav = obj;
      break;
    }
    if(!(obj->~IsWeapon()))
    {
      i++;
      mode = -1;
      continue;
    }
    if(mode && !fFireModes)
    {
      i++;
      mode = -1;
      continue;
    }
    if(!(obj->GetFMData(FM_Name, mode)))
    {
      i++;
      mode = -1;
      continue;
    }
    if(mode == obj->GetFireMode() && mode)
      continue;
    //Waffe lässt sich nur im Zielen abfeuern? Level darf nicht 1 sein und der Clonk muss Zielen/Laufen
    if(obj->GetFMData(FM_Aim, mode) > 0)
      if(iLevel == 1 || (!WildcardMatch(GetAction(), "*Walk*") && !WildcardMatch(GetAction(), "*Aim*")))
        continue;
    //Keine Munition verfügbar
    if(!NoAmmo() && !(obj->GetCharge()) && !GetAmmo(obj->GetFMData(FM_AmmoID, mode)))
      continue;

    //EMP nur wenn der Gegner darauf reagiert
    if(obj->GetBotData(BOT_EMP, mode))
      if(!(pTarget->~IsMachine()))
        continue;

    //Objekt braucht lange zum laden und ist leer/lädt nach
    if(obj->GetBotData(BOT_Power, mode) == BOT_Power_LongLoad && (obj->IsReloading() || !(obj->GetCharge())))
      if(!fEmptyLongLoad)
        continue;

    if(!fav)
    {
      fav = obj;
      type = fav->GetBotData(BOT_DmgType, mode);
      favmode = mode;
    }
    else 
    {
      //Schadenstypresistenzen und "Stärke der Waffe" vergleichen
      if(pTarget->~OnDmg(0, obj->GetBotData(BOT_DmgType, mode)) < pTarget->~OnDmg(0, type) &&
        fav->GetBotData(BOT_Power, favmode) - 1 <= obj->GetBotData(BOT_Power, mode))
      {
        fav = obj;
        type = fav->GetBotData(BOT_DmgType);
        favmode = mode;
      }
      //Bei gleichgroßer Schadenstypresistenz...
      if(pTarget->~OnDmg(0, obj->GetBotData(BOT_DmgType, mode)) == pTarget->~OnDmg(0, type))
      {
        //Reichweite zu gering?
        if(fav->GetBotData(BOT_Range, favmode) < dist)
        {
          //Neues Objekt hat höhere Reichweite?
          if(obj->GetBotData(BOT_Range, mode) > dist)
          {
            fav = obj;
            type = obj->GetBotData(BOT_DmgType, mode);
            favmode = mode;
          }
        }
        //Objektreichweite ist hoch genug
        if(obj->GetBotData(BOT_Range, mode) > dist)
        {
          //Neues Objekt ist "stärker" oder die favorisierte Waffe braucht lange zum Laden und ist leer/lädt nach
          if(fav->GetBotData(BOT_Power, favmode) < obj->GetBotData(BOT_Power, mode))
          {
            //Neues Objekt braucht nicht lange zum laden
            if(obj->GetBotData(BOT_Power, mode) != BOT_Power_LongLoad)
            {
              fav = obj;
              type = fav->GetBotData(BOT_DmgType);
              favmode = mode;
            }
            //Neues Objekt ist nicht leer / lädt nicht nach
            else if(obj->GetCharge() != 0 && !(obj->IsReloading()))
            {
              fav = obj;
              type = fav->GetBotData(BOT_DmgType);
              favmode = mode;
            }
          }
        }
      }

      if(fav->GetBotData(BOT_Range, favmode) >= dist)
        if(preftype == type)
          if(fav->GetBotData(BOT_Power, favmode) >= BOT_Power_3)
            break;
    }
  }
  //Auswählen
  if(!fav)
    return;
  //Feuermodus wechseln?
  if(fFireModes)
    if(favmode && favmode != fav->GetFireMode())
      fav->SetFireMode(favmode);
  if(ContentsCount() == 1 || fav == Contents())
    return 1;

  ShiftContents(0, 0, fav->GetID(), true);
  Contents()->~ResumeReload();
  return true;
}

public func GetPrefDmgType(object pTarget)
{
  var min = pTarget->~OnDmg(0, DMG_Projectile);
  var type = DMG_Projectile;
  if(pTarget->~OnDmg(0, DMG_Melee) < min)
  {
    type = DMG_Melee;
    min = pTarget->~OnDmg(0, type);
  }
  if(pTarget->~OnDmg(0, DMG_Fire) < min)
  {
    type = DMG_Fire;
    min = pTarget->~OnDmg(0, type);
  }
  if(pTarget->~OnDmg(0, DMG_Explosion) < min)
  {
    type = DMG_Explosion;
    min = pTarget->~OnDmg(0, type);
  }
  if(pTarget->~OnDmg(0, DMG_Energy) < min)
  {
    type = DMG_Energy;
    min = pTarget->~OnDmg(0, type);
  }
  if(pTarget->~OnDmg(0, DMG_Bio) < min)
    type = DMG_Bio;

  return type;
}

/* Waffenbehandlung wenn nicht im Kampf */

public func CheckIdleWeapon()
{
  if(Contents())
  {
    //Bei BR-Bombe nichts unternehmen
    if(Contents()->GetID() == GBRB) return;
    if(Contents()->~RejectShift()) return;
  }
  //Keine Waffen im Inventar
  if(!CustomContentsCount("IsWeapon")) return;
  //Nachladende Waffe in der Hand
  if(Contents()->~IsWeapon())
    if(Contents()->IsReloading() || Contents()->~IsRecharging())
      return true;
  //Inventar nach Waffe durchsuchen, die man Nachladen könnte
  for(var i=0, mode=1, obj; obj = Contents(i) ; mode++)
  {
    //Keine Waffe
    if(!(obj->~IsWeapon()))
    {
      i++;
      mode = 0;
      continue;
    }
    //Waffe hat gar nicht so viele Modi
    if(!(obj->GetFMData(FM_Name, mode)))
    {
      i++;
      mode = 0;
      continue;
    }
    //Waffe ist voll geladen
    if(obj->GetAmmo(obj->GetFMData(FM_AmmoID, mode)) >= obj->GetFMData(FM_AmmoLoad, mode) / 2)
    {
      i++;
      mode = 0;
      continue;
    }
    //EMP-Modi erstmal nicht laden
    if(obj->GetBotData(BOT_EMP, mode)) continue;
    // Waffe ist nachladbar
    if(CheckAmmo(obj->GetFMData(FM_AmmoID, mode), obj->GetFMData(FM_AmmoLoad, mode) - obj->GetAmmo(obj->GetFMData(FM_AmmoID, mode))))
    {
      //mode = obj->GetFireMode();
      break;
    }
    //Nächsten Feuermodus prüfen
    if(mode == obj->GetFireMode()) continue;
    if(CheckAmmo(obj->GetFMData(FM_AmmoID, mode), obj->GetFMData(FM_AmmoLoad, mode)))
      break;
  }
  //Nichts gefunden
  if(!Contents(i)) return;

  //Waffe wechseln
  if(ContentsCount() != 1 && Contents() != obj)
    SelectInventory(GetID(obj));
  //Feuermodus wechseln
  if(obj->GetFireMode() != mode)
    obj->SetFireMode(mode);
  //Munition aktualisieren
  /*
  if(!Contents()->~IsRecharging())
    Schedule("Control2Contents(\"ControlThrow\")", 1);
  */
  Contents()->~Reload();
  return(1);
}

protected func MacroComMoveTo()
{
  var x, y;
  x = GetMacroCommand(0, 2);
  y = GetMacroCommand(0, 3);
  if(!inherited())
    return;
  SetCommand(this, "MoveTo", 0, x, y);
  return;
}