/*-- NUR OPEN BETA --*/

#strict 2

#appendto MAVE

public func FxFlyingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Bei Wasserkontakt Schaden nehmen
  if(GBackLiquid(0, 0) && (water_damage = !water_damage))
    DoDamage(1);

  if(GetAction() != "Flying") 
    return;

  SetController(GetOwner());

  //Flugverhalten
  if(iXDir < iXTendency)
    iXDir+= 1 - (fIsAiming && !(iEffectTime % 3));
  if(iXDir > iXTendency)
    iXDir-= 1 - (fIsAiming && !(iEffectTime % 3));
  if(iYDir < iYTendency)
    iYDir+= 1 - (fIsAiming && !(iEffectTime % 3));
  if(iYDir > iYTendency)
    iYDir-= 1 - (fIsAiming && !(iEffectTime % 3)) + (iYDir>iSpeed);

  if(GetY() <= GetDefCoreVal("Offset", "DefCore", MAVE, 1) * -1 && iYTendency <= 0)
  {
    iYTendency = 0;
    iYDir = BoundBy((GetY() + GetDefCoreVal("Offset", "DefCore", MAVE, 1))*-1, 1, 10);
  }

  if(!iYTendency && !iYDir)
    iYDir += Sin(iEffectTime * 8, 2);

  //C4 verlangsamt den Flug
  CountC4();
  
  //Widerstand im Wasser
  var resistance = 0;
  if(GBackLiquid())
    resistance = 2;

  SetXDir(iXDir - (iC4Count + resistance) * 10 * iXDir / iSpeed);

  if(iYDir<=0)
    SetYDir(iYDir - (iC4Count + resistance) * 10 * iYDir / iSpeed - 2);
  else
    SetYDir(iYDir + (iC4Count - resistance) * 10 * iYDir / iSpeed - 2);

  //Je nach Flugrichtung drehen
  if(iXTendency == 0)
    iBank -= iBank / Abs(iBank);
  else
    if(Abs(iBank) < iBankMax)
      iBank += iXTendency / Abs(iXTendency);

    SetObjDrawTransform(1000, -iBank*20, 0, iBank*20, 1000, 0, this);

  //Partikeleffekte
  for(var i = 0; i < 5; i++)
  {
    var iXParticle = RandomX(-4, 4) + GetXDir() * 3 / 4;
    var iYParticle = RandomX(-9, 9) + 10 - (iYTendency - iYDir) * 3 / 4;

    //Im Wasser: Luftblasen
    if(GBackLiquid(0, 0))
    {
      var bubble = CreateObject(FXU1, 6 - GetXDir() / 10,  - 8*(iYTendency > 0) + 2*Sgn(iBank-Sgn(iBank)) - iYParticle / 6, -1);
      if(bubble)
      {
        bubble->SetXDir(iXParticle);
        bubble->SetYDir(iYParticle);
      }
      bubble = CreateObject(FXU1, -7 - GetXDir() / 10,  - 8*(iYTendency > 0) - 2*Sgn(iBank-Sgn(iBank)) - iYParticle / 6, -1);
      if(bubble)
      {
        bubble->SetXDir(iXParticle);
        bubble->SetYDir(iYParticle);
      }
      //Objekte sparen...
      break;
    }
    else
    {
      CreateParticle("PSpark", 6 - GetXDir() / 10, - 8*(iYTendency > 0) + 2*Sgn(iBank-Sgn(iBank)) - iYParticle / 6, iXParticle, iYParticle, 15, RGBa(200, 200, 255, 35));
      CreateParticle("PSpark", -7 - GetXDir() / 10, - 8*(iYTendency > 0) - 2*Sgn(iBank-Sgn(iBank)) - iYParticle / 6, iXParticle, iYParticle, 15, RGBa(200, 200, 255, 35));
    }
  }

  //Nachladen und Sensorchecks (alle 5 Frames)
  if(!(iEffectTime % 5))
  {
    //Waffe laden sofern nötig
    if((GetAmmo(GetAttWeapon()->GetFMData(FM_AmmoID), GetAttWeapon()) < GetAttWeapon()->GetFMData(FM_AmmoUsage)) && !GetAttWeapon()->IsReloading())
      Reload();

    //Sicherheitsprüfung
    if(!pItem || Contained(pItem) != this)
    {
      pItem = 0;
      iItemType = 0;
      SetPhase(iItemType);
    }
    
    //Blinklicht (alle 30 Frames)
    if(!(iEffectTime % 30))
    {
      //Farbe ermitteln
      if(GetTeam())
        var rgb = GetTeamColor(GetTeam());
      else
        if(GetOwner())
          var rgb = GetPlrColorDw(GetOwner());
        else
          var rgb = RGB(255, 255, 255);
      CreateParticle("FlashLight", 0, 4, 0, 0 , 45, rgb, this);
    }

    //Ausrüstung nutzt Statusbalken?
    if(Inside(iItemType, 1, 6) && !GetEffect("Bars", this))
      AddEffect("Bars", this, 1, 1, this);

    //Ausrüstungsspezifische Aktionen
    if(iItemType == 0 && !(iEffectTime % 25)) Sense();			//Standard: Sensorball
    if(iItemType == 1 && !(iEffectTime % 60)) AMP(false);		//MTP
    if(iItemType == 2 && !(iEffectTime % 20)) FAP(false, iEffectTime);	//EHP
    if(iItemType == 3) BlowTorch(false);				//Schweißbrenner
  }

  //Echtzeitanforderungen, daher außerhalb
  if(iItemType == 5) HardKill();		//Sprengfalle
  if(iItemType == 6) ShockPaddles();		//Defibrillator

  if(iEffectTime <= 2)
  {
    if(iItemType == 1) AMP(true);
    if(iItemType == 2) FAP(true, iEffectTime);
    if(iItemType == 3) BlowTorch(true);
  }

  //Namensanzeige für Verbündete
  for(var first = true, iFlags, i = 0; i < GetPlayerCount(); i++)
  {
    var iPlr = GetPlayerByIndex(i);
    if(pMAVStation && !Hostile(GetOwner(), iPlr) && GetOwner(this) != iPlr)
    {
      if(first)
        first = false;
      else
        iFlags = MSG_Multiple;
      var szStr = Format("@%s (%s)", GetName(pMAVStation->GetUser()), GetPlayerName(GetOwner()));
      CustomMessage(szStr, cur_Attachment, iPlr, 0, 0, SetRGBaValue(GetPlrColorDw(GetOwner()), 128), 0, 0, iFlags);
    }
  }

  //Schadensverhalten
  DamageChecks();

  //Waffensteuerung
  if(fIsAiming)
  {
    //Waffe vorhanden?
    if(!GetAttWeapon()) return;
    //Funktionstüchtig?
    if(EMPShocked()) return;
    if(IsDestroyed()) return;

    cur_Attachment->SetTeam(GetTeam());

    //Überdrehung nach links und rechts verhindern
    if(AimAngle() <= MaxRotLeft() && iPat_Dir < 0)
    {
      iPat_Dir = 0;
    }
    else if(AimAngle() >= MaxRotRight() && iPat_Dir > 0)
    {
      iPat_Dir = 0;
    }

    iAimAngle += iPat_Dir;

    //Fadenkreuz platzieren
    if(crosshair)
    {
      if(AimAngle()+GetR() <= 360)
        crosshair->SetAngle(AimAngle()-GetR()+360);
      else
        crosshair->SetAngle(AimAngle()-GetR());
    }

    if(!pLaser)
      pLaser = CreateObject(LRDT,0,0,GetOwner(this));

    var xPos = GetX(), yPos = GetY(), x = GetX(), y = GetY(), xdir = Sin(AimAngle(), 30), ydir = Cos(AimAngle(), -30);
    var gravity = GetGravity();
    var fStart = false;

    SetGravity(0);
    if (!SimFlight(x, y, xdir, ydir))
      pLaser->Stop();
    else 
      if(!pLaser->Active())
        fStart = true;
    SetGravity(gravity);

    xdir = Sin(AimAngle(), 3000);
    ydir = Cos(AimAngle(), -3000);

    var pEnemy;

    if(pLaser->Active() || fStart)
      pEnemy = FindObject2(Find_OnLine(0, 0, x - xPos, y - yPos), Find_Hostile(GetOwner(this)), Find_NoContainer(), Find_Or(Find_OCF(OCF_Alive), Find_Func("IsBulletTarget", GetID(), this, this), Find_Func("IsCMCVehicle")), Sort_Distance(0, 0));
    else
      pEnemy = FindObject2(Find_OnLine(0, 0, xdir, ydir), Find_Hostile(GetOwner(this)), Find_NoContainer(), Find_Or(Find_OCF(OCF_Alive), Find_Func("IsBulletTarget", GetID(), this, this), Find_Func("IsCMCVehicle")), Sort_Distance(0, 0));

    //Feinderkennung
    if(pEnemy)
    {
      if(!pLaser->Active())
        fStart = true;

      x = GetX(pEnemy);
      y = GetY(pEnemy);

      var xLeft = GetDefCoreVal("Offset", "DefCore", GetID(pEnemy), 0) + x;
      var xRight = GetDefCoreVal("Width", "DefCore", GetID(pEnemy)) + GetDefCoreVal("Offset", "DefCore", GetID(pEnemy), 0) + x;

      var yUp = GetDefCoreVal("Offset", "DefCore", GetID(pEnemy), 1) + y;
      var yDown = GetDefCoreVal("Height", "DefCore", GetID(pEnemy)) + GetDefCoreVal("Offset", "DefCore", GetID(pEnemy), 1) + y;

      if(Inside(xPos, xLeft, xRight) && Inside(yPos, yUp, yDown))
      {
        x = xPos;
        y = yPos;
      }
      else
      {
        var xOff, yOff;

        if(xPos > x)
          xOff = xRight;
        else
          xOff = xLeft;

        if(yPos > y)
          yOff = yDown;
        else
          yOff = yUp;

        if(Inside(xPos, Min(x, xOff), Max(x, xOff)))
        {
          x = Sin(AimAngle(), (yOff - yPos) * 1000 / (-Cos(AimAngle(), 1000))) + xPos;
          y = -Cos(AimAngle(), (yOff - yPos) * 1000 / (-Cos(AimAngle(), 1000))) + yPos;
        }
        else
          if(Inside(yPos, Min(y, yOff), Max(y, yOff)))
          {
            x = Sin(AimAngle(), (xOff - xPos) * 1000 / (Sin(AimAngle(), 1000))) + xPos;
            y = -Cos(AimAngle(), (xOff - xPos) * 1000 / (Sin(AimAngle(), 1000))) + yPos;
          }
        else
          if((Angle(xPos, yPos, xOff, yOff) >= 180 && Angle(xPos, yPos, xOff, yOff) < AimAngle()) || (Angle(xPos, yPos, xOff, yOff) <= 180 && Angle(xPos, yPos, xOff, yOff) > AimAngle()))
          {
            x = Sin(AimAngle(), (yOff - yPos) * 1000 / (-Cos(AimAngle(), 1000))) + xPos;
            y = -Cos(AimAngle(), (yOff - yPos) * 1000 / (-Cos(AimAngle(), 1000))) + yPos;
          }
        else
        {
          x = Sin(AimAngle(), (xOff - xPos) * 1000 / (Sin(AimAngle(), 1000))) + xPos;
          y = -Cos(AimAngle(), (xOff - xPos) * 1000 / (Sin(AimAngle(), 1000))) + yPos;
        }
      }

  }

  //Laser zeichnen
  if(!pBeam)  
    pBeam = CreateObject(LRBM, 0, 0, GetOwner(this));
  else
    pBeam->SetPosition(xPos, yPos);

  //Sichtbarkeit nur für Besitzer und Verbündete
  pBeam->SetVisibility(VIS_Owner | VIS_Allies);

  //Laser passend strecken
  if(pEnemy || pLaser->Active() || fStart)
    pBeam->SetObjDrawTransform(100 * Distance(xPos, yPos, x, y), 0, -453 * Distance(xPos, yPos, x, y), 0, 1000, 0);
  else
    pBeam->SetObjDrawTransform(100 * Distance(xPos, yPos, xPos + xdir/3, yPos + ydir/3), 0, -453 * Distance(xPos, yPos, xPos + xdir/3, yPos + ydir/3), 0, 1000, 0);

  pBeam->SetR(AimAngle()+90);
  SetPosition(x, y, pLaser);
  if(fStart)
    pLaser->Start();
  }

  //Feindliche Clonks zum Rammen suchen
  if((Abs(GetXDir()) + Abs(GetYDir()) >= 25) && !GetEffect("MeleeCooldown", this))
  {
    var strike;
    var target = FindObject2(Find_AtPoint(0, 0), Find_Hostile(GetOwner(this)), Find_NoContainer(), Find_OCF(OCF_Alive), Find_Not(Find_Func("HitExclude", this)));
    if(target)
    {
      //Ziel am kriechen?
      if(WildcardMatch(GetAction(target),"*Crawl*"))
      {
        //Erhöhten Schaden verursachen
        DoDmg(30, DMG_Melee, target, 0, GetController(this)+1, GetID());
        //Ziel zum Aufstehen zwingen
        ObjectSetAction(target, "KneelUp");
      }
      else
      {
        //Schaden verursachen
        DoDmg(20, DMG_Melee, target, 0, GetController(this)+1, GetID());

        //Ziel schleudern
        var pwr = 18, angle = 45, dir = iXDir / Abs(iXDir);
        if(target && GetProcedure(target) != "SWIM")
        {
          if(!dir)
             dir--;
          SetXDir(Sin(angle * dir, pwr), target, 10);
          SetYDir(-Cos(angle * dir, pwr), target, 10);
          ObjectSetAction(target, "Tumble");
        }
      }
      
      //Achievement-Fortschritt (Can't touch this)
      if(!GetAlive(target) || IsFakeDeath(target))
        DoAchievementProgress(1, AC41, GetOwner());

      //Auch das MAV nimmt Schaden
      DoDmg(10, DMG_Melee, this, 0, GetController(this)+1, GetID());
      iXDir /= 2;
      iYDir /= 2;
      AddEffect("MeleeCooldown", this, 1, 30);

      //Sound
      Sound("WPN2_Punch*.ogg");
      Sound("BKHK_RotorHit*.ogg");
      strike = true;
    }

    var xOff = GetDefCoreVal("Offset", "DefCore", MAVE, 0);
    var yOff = GetDefCoreVal("Offset", "DefCore", MAVE, 1);

    //Andere Ziele suchen
    if(target = FindObject2(Find_Func("IsMeleeTarget", this),
      Find_Or(Find_AtPoint(0, 0), Find_AtRect(-xOff + (GetXDir() / 2)*(iXDir<0), -yOff + (GetYDir() / 2)*(iYDir<0), xOff*2 + (GetXDir() / 2)*(iXDir>0), yOff*2 + (GetYDir() / 2)*(iYDir>0))), Find_Not(Find_Func("HitExclude", this))))
    {
      DoDmg(20, DMG_Melee, target, 0, GetController(this)+1, GetID());
      AddEffect("MeleeCooldown", this, 1, 30);

      //Gesonderter Schaden bei feindlichen MAVs
      if(target && target->~IsMAV())
      {
        target->DoDmg(20, DMG_Melee, this, 0, GetController(target)+1, GetID(target));
        AddEffect("MeleeCooldown", target, 1, 30);
      }

      //Effekte
      if(!strike)
      {
        Sound("HeavyHit*.ogg");
        if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",2,40,0,0,0,50,60);
        Sparks(Random(2)+2,RGB(255,255,Random(5)+255));
      }
    }
  }
}

/* Defibrillator */

public func ShockPaddles()
{
  //Schwerverletzte und Feinde suchen
  if(pItem->Ready() && 
    FindObject2(Find_AtPoint(0 - iXDir/5 - Sgn(iXDir), 0 - iYDir/5 - Sgn(iYDir)),
  							Find_OCF(OCF_NotContained),
  							Find_Or(
  							Find_And(Find_ID(FKDT),				//Schwerverletzter?
  							Find_Allied(GetOwner()),			//Verbündet?
  							Find_Not(Find_Func("RejectReanimation"))),	//Wiederbeleben erlaubt?
  							Find_And(Find_OCF(OCF_Alive),			//Lebendig?
  							Find_Hostile(GetOwner()),     //Feind?
  							Find_Not(Find_Func("HitExclude", this))))))	  //Darf getroffen werden?
    //Defibrillator aktivieren
    pItem->Activate(this);

  //Statusbalken aktualisieren
  if(!ChargeBar)
  {
    ChargeBar = CreateObject(SBAR, 0, 0, GetOwner());
    ChargeBar->Set(this, RGB(77, 229, 0), BAR_Ammobar, 0, 0, SM13);
  }
  else
    ChargeBar->Update(LocalN("charge", pItem) * 100 / pItem->MaxEnergy(), false);
}

/* Schweißbrenner */

public func BlowTorch(bool statusOnly)
{
  var pItem = LocalN("pItem", this);

  //Statusbalken aktualisieren
  if(!ChargeBar)
  {
    ChargeBar = CreateObject(SBAR, 0, 0, GetOwner());
    ChargeBar->Set(this, RGB(77, 229, 0), BAR_Ammobar, 0, 0, SM12);
  }
  else
    ChargeBar->Update(LocalN("charge", pItem) * 100 / pItem->MaxEnergy(), false);

  if(statusOnly)
    return;

  //Eventuellen Cooldown verringern
  if(living_dmg_cooldown)
    living_dmg_cooldown--;

  var used;

  if(LocalN("charge", pItem) < 2) return Sound("BWTH_Repair.ogg", false, this, 100, 0, -1);
  
  //Angreifbare Objekte suchen
  var obj = FindObject2(Find_Func("IsMeleeTarget", this),	//Angreifbar?
  		Find_Exclude(this),				//Kein Self-Attack
  		Find_AtRect(-10,-10,10,10));
  if(obj)
  {
    //Objekt beschädigen
    DoDmg(3, DMG_Fire, obj);

    LocalN("charge", pItem) = BoundBy(LocalN("charge", pItem)-2, 0, pItem->MaxEnergy());
    used = true;
  }
  //Entschärfbare Objekte suchen
  var obj = FindObject2(Find_Func("IsDefusable"),	//Entschärfbar?
  		Find_Hostile(GetOwner(this)),		//Feindlich?
  		Find_NoContainer(),			//Nicht verschachtelt?
  		Find_AtRect(-10,-10,10,10));
  if(obj)
  {
    if(obj->~RTDefuse(this))
      //Punkte bei Belohnungssystem (Entschärfung)
      DoPlayerPoints(BonusPoints("TechnicalTask"), RWDS_TeamPoints, GetOwner(this), this, IC15);

    LocalN("charge", pItem) = BoundBy(LocalN("charge", pItem)-2, 0, pItem->MaxEnergy());
    used = true;
  }
  //Reparierbare Objekte suchen
  obj = FindObject2(Find_Or(Find_And(Find_Func("IsRepairable"),		//Reparierbar?
  	Find_Or(
  	Find_Func("GetDamage"),						//Beschädigt?
  	Find_Hostile(GetOwner(this))),					//Feindlich?
  	Find_Exclude(this)),						//Kein Self-Repair
  	Find_And(
  	Find_OCF(OCF_Alive),
  	Find_Hostile(GetOwner(this)),
  	Find_NoContainer()),						//Nicht verschachtelt?
  	Find_Func("IsFakeRepairable", GetOwner(this))),			//Konsolen?
  	Find_AtRect(-10,-10,10,10),
  	Find_Not(Find_Func("HitExclude", this)));
  if(obj)
  {
    //Konsolen reparieren / beschädigen
    if(obj->~IsFakeRepairable())
      obj = obj->GetRealRepairableObject();

    if(Hostile(GetOwner(obj), GetOwner(this)))
    {
      if(obj->~IsRepairable())
      {
        //Feindliche Fahrzeuge beschädigen
        DoDmg(5, DMG_Fire, obj);

        LocalN("charge", pItem) = BoundBy(LocalN("charge", pItem)-2, 0, pItem->MaxEnergy());
      }
      else
      {
        if(!living_dmg_cooldown)
        {
          //Feindliche Lebewesen verletzen
          DoDmg(12,DMG_Fire,obj);

          if(!GetAlive(obj) || IsFakeDeath(obj))
            //Achievement-Fortschritt (I'll fix you up?)
            DoAchievementProgress(1, AC32, GetOwner(this));

          living_dmg_cooldown = 7;
        }
        if(!Random(7))
          Sound("SharpnelImpact*.ogg");

        LocalN("charge", pItem) = BoundBy(LocalN("charge", pItem) - 2, 0, pItem->MaxEnergy());
      }
    }
    else
    {
      if(!obj->~RejectRepair())
      {
         //Fahrzeug reparieren
         DoDamage(-2, obj);

         if(!Hostile(GetOwner(obj), GetOwner(this)) && GetOwner(obj) != GetOwner(this) && GetOwner(obj) != NO_OWNER)
         {
           //Achievement-Fortschritt (Wicked Engineer)
           DoAchievementProgress(2, AC33, GetOwner(this));
         }

         //Callback
         if(GetDamage(obj) == 0)
           obj->~IsFullyRepaired();
         else
           obj->~OnRepairing(this);
      }

      if(!Hostile(GetOwner(obj), GetOwner(Contained())) && GetOwner(obj) != GetOwner(this) && LocalN("iRepaired", pItem)++ >= 50)
      {
         //Punkte bei Belohnungssystem (Reparatur)
         DoPlayerPoints(BonusPoints("Repair"), RWDS_TeamPoints, GetOwner(this), this, IC15);
         LocalN("iRepaired", pItem) = 0;
      }
    LocalN("charge", pItem) = BoundBy(LocalN("charge", pItem) - 2, 0, pItem->MaxEnergy());
    }

    used = true;
  }

  if(used)
  {
    //Effekte
     CreateParticle("RepairFlame", -1, 7, 0, 2-Random(2), 80, RGB(0,100,250));
     if(GetEffectData(EFSM_BulletEffects) >1 && !Random(2))
       AddLightFlash(80, -1, 7, RGB(0,140,255));
     if(!Random(2))
       Sparks(8+Random(4), RGB(100,100,250), RandomX(-5, 5), RandomX(-5,5));

     Sound("BWTH_Repair.ogg", false, this, 100, 0, 1);
   }
   else
    Sound("BWTH_Repair.ogg", false, this, 100, 0, -1);
}
