/*-- Waffenscript-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto WPN2

local iAttachment, pLaser, pBeam, pLight;

static const AT_NoAttachment		= 0;	//Kein Waffenaufsatz
static const AT_ExtendedMag		= 1;	//Erweitertes Magazin
static const AT_Bayonet			= 2;	//Bajonett
static const AT_Laserpointer		= 4;	//Laserpointer
static const AT_Silencer		= 8;	//Schalldämpfer
static const AT_Flashlight		= 16;	//Taschenlampe


func PermittedAtts()
{
  return AT_NoAttachment;
}

func GetAttachment()
{
  return iAttachment;
}

func GetLaser()
{
  return pLaser;
}

/* Waffenaufsatz festlegen */

func SetAttachment(int iValue)
{
  if(iValue && !(PermittedAtts() & iValue)) return -1;

  //Eventuell vorhandene Effekte entfernen
  if(GetEffect("LaserDot", this))	RemoveEffect("LaserDot", this);
  if(GetEffect("Silencer", this))	RemoveEffect("Silencer", this);
  if(GetEffect("Flashlight", this))	RemoveEffect("Flashlight", this);

  //Feuermodus zurücksetzen
  SetFireMode(1);

  //Bei erweitertem Magazin: Waffe entladen
  if(iAttachment == AT_ExtendedMag) Empty();

  //Eventuell entsprechenden Effekt setzen
  var iTemp = iAttachment;
  iAttachment = iValue;
  if(iAttachment == AT_Laserpointer)	AddEffect("LaserDot", this, 1, 1, this);
  if(iAttachment == AT_Silencer)	AddEffect("Silencer", this, 1, 1, this);
  if(iAttachment == AT_Flashlight)	AddEffect("Flashlight", this, 1, 1, this);

  //Waffenaufsatz-Symbol setzen
  SetGraphics(0,0,AttachmentIcon(iAttachment),2,GFXOV_MODE_Picture);    
  SetObjDrawTransform(500,0,10000,0,500,10000, 0, 2);

  //Waffen-HUD aktualisieren
  if(Contained()) Contained()->~UpdateCharge(true);

  return iTemp;
}

/* Taschenlampe */

public func SensorDistance()		{return 200;}
public func BlindEffectDistance()	{return 150;}
public func FlashlightAngle()		{return 30;}

public func FxFlashlightTimer(object pTarget, int iNr, int iTime)
{
  var user = this->~GetUser(), prevUser = EffectVar(1, pTarget, iNr);
  var light = EffectVar(0, pTarget, iNr);

  var deactivate = false;
  if(!user || Contents(0, user) != this || Contained(user) || !GetEffect("ShowWeapon", user))
    deactivate = true;

  //Licht bei Benutzerwechsel entfernen
  if((user != prevUser) && light)
    RemoveObject(light);

  //Spieler hat schon eine Taschenlampe ausgerüstet
  if((deactivate && light) || (user && user->~HasGear(0, FLSH)))
  {
    if(light)
      SetVisibility(VIS_None, light);
  }
  else if(user)
  {
    var angle = user->~AimAngle();
    var dir = GetDir(user) * 2 - 1;

    //Wackel- und Schwenkeffekte beim Laufen/Springen
    if((WildcardMatch(GetAction(user), "*Walk*") && !Inside(GetXDir(user), -10, 10)) || WildcardMatch(GetAction(user), "*Kneel*") || WildcardMatch(GetAction(user), "*Flat*"))
    {
      //Ruckeleffekt bei Landung
      if(EffectVar(4, pTarget, iNr))
      {
        if(WildcardMatch(GetAction(user), "*Kneel*") || WildcardMatch(GetAction(user), "*Flat*"))
          EffectVar(4, pTarget, iNr) = 2;

        EffectVar(5, pTarget, iNr)++;
        EffectVar(2, pTarget, iNr) = Random(5 + 4*(EffectVar(4, pTarget, iNr)-1))-2 * EffectVar(4, pTarget, iNr);

        if(EffectVar(5, pTarget, iNr) >= (EffectVar(4, pTarget, iNr)*5))
          EffectVar(4, pTarget, iNr) = EffectVar(5, pTarget, iNr) = 0;
      }
      //Leichtes Wackeln beim Laufen
      else if(!Inside(GetXDir(user), -10, 10))
      {
        if(iTime % 5)
          EffectVar(3, pTarget, iNr) = 0;
        else
        {
          if(EffectVar(2, pTarget, iNr) >= 1)
            EffectVar(3, pTarget, iNr) = -1;
          else if(EffectVar(2, pTarget, iNr) <= -1)
            EffectVar(3, pTarget, iNr) = +1;
          else if(!EffectVar(3, pTarget, iNr))
            EffectVar(3, pTarget, iNr) = -1;
        }
      }
    }
    //Taschenlampe nach oben/unten schwenken während des Sprungs
    else if(WildcardMatch(GetAction(user), "*Jump*"))
    {
      if(GetYDir(user) > 0 && EffectVar(2, pTarget, iNr) > -5)
        EffectVar(3, pTarget, iNr) = -1;
      else if(GetYDir(user) < 0 && EffectVar(2, pTarget, iNr) < 5)
        EffectVar(3, pTarget, iNr) = +1;
      else
        EffectVar(3, pTarget, iNr) = 0;

      if(!GetYDir(user))
        EffectVar(3, pTarget, iNr) = 0;

      EffectVar(4, pTarget, iNr) = true;
    }
    //Zusatzrotation zurücksetzen, falls nicht am Springen/Laufen/Landen
    else if(EffectVar(2, pTarget, iNr) != 0)
    {
      EffectVar(4, pTarget, iNr) = false;
      EffectVar(3, pTarget, iNr) = BoundBy(EffectVar(2, pTarget, iNr) * -2, -1, 1);
    }
    else
      EffectVar(3, pTarget, iNr) = 0;

    var handr = this->~HandR() * dir;
    angle += handr + (EffectVar(2, pTarget, iNr) += EffectVar(3, pTarget, iNr)) * dir;

    if(!light)
    {
      light = EffectVar(0, pTarget, iNr) = AddLightCone2(1000, RGBa(255, 255, 220, 40), user);
      light->ChangeSizeXY(1400, 6000);
      light->Lock();
    }

    if(deactivate)
      SetVisibility(VIS_None, light);
    else
      SetVisibility(VIS_All, light);

    var x, y;
    if(GetEffect("ShowWeapon", user))
    {
      user->WeaponBegin(x, y);

      //Lichtkegel wird nicht attached
      SetPosition(GetX(user), GetY(user), light);
      light->ChangeOffset(x*dir, y * -1, true);
      light->ChangeR(angle);
    }
  }

  EffectVar(1, pTarget, iNr) = user;

  if(deactivate || iTime % 4)
    return;

  var iAngleMin = angle-FlashlightAngle()/2;
  var iAngleMax = angle+FlashlightAngle()/2;

  //Zu markierende Gefahren suchen
  for (var pObj in FindObjects(Find_Distance(SensorDistance()),			//In Reichweite
  		Find_Hostile(GetController(user)),				//Nur feindliche Objekte markieren
  		Find_Or(Find_OCF(OCF_Alive), Find_Func("IsDetectable")),	//Lebewesen oder als identifizierbar markiert
  		Find_NoContainer(),						//Im Freien
  		Find_Exclude(this)))						//Selber ausschließen
  {
    if(pObj == user)
      continue;

    //Objekt im Suchkegel?
    var x = GetX(), y = GetY(), ox = GetX(pObj), oy = GetY(pObj);
    var target_angle = Normalize(Angle(x, y, ox, oy), -180);

    if(iAngleMax < 180 && (target_angle < iAngleMin || target_angle > iAngleMax))
      continue;
    else if(iAngleMax >= 180 && (target_angle < iAngleMin && target_angle > iAngleMax-360))
      continue;
    
    if(pObj->~IsClonk() && PathFree(x, y, ox, oy) && Distance(x, y, ox, oy) < BlindEffectDistance())
    {
      if(GetEffect("FlashlightBlindness", pObj))
        EffectCall(pObj, GetEffect("FlashlightBlindness", pObj), "Refresh");
      else
        AddEffect("FlashlightBlindness", pObj, 100, 1, 0, WPN2);
    }

    //Bereits markierte Objekte auslassen
    var tag;
    if(tag = FindObject2(Find_ActionTarget(pObj), Find_ID(SM08), Find_Allied(GetController(user))))
    {
      tag->~RefreshRemoveTimer();
      continue;
    }

    //Ansonsten markieren
    CreateObject(SM08, GetX(pObj), GetY(pObj), GetController(user))->Set(pObj, this, GetOCF(pObj) & OCF_Alive, 26);
  }

  return true;
}

public func FxFlashlightStop(object pTarget, int iNr, int iReason, bool fTemp)
{
  if(fTemp)
    return;

  //Licht löschen
  if(EffectVar(0, pTarget, iNr))
    RemoveObject(EffectVar(0, pTarget, iNr));

  return true;
}

public func FxFlashlightBlindnessStart(object pTarget, int iNr, temp)
{
  if(temp)
    return;

  EffectVar(0, pTarget, iNr) = ScreenRGB(pTarget, RGBa(255, 255, 255, 254), 0, 0, false, SR4K_LayerLight);
  EffectVar(1, pTarget, iNr) = 6;
}

static const WPN2_Flashlight_MinAlpha = 130;

public func FxFlashlightBlindnessTimer(object pTarget, int iNr)
{
  var rgb = EffectVar(0, pTarget, iNr);
  if(!rgb)
    rgb = EffectVar(0, pTarget, iNr) = ScreenRGB(pTarget, RGBa(255, 255, 255, 254), 0, 0, false, SR4K_LayerLight);

  if(rgb->GetAlpha() < WPN2_Flashlight_MinAlpha)
    return;

  if(--EffectVar(1, pTarget, iNr) <= 0)
  {
    if(GetEffect("IntFlashbang", pTarget))
      return;

    rgb->DoAlpha(-5, WPN2_Flashlight_MinAlpha, 255);
  }
  else
    rgb->DoAlpha(+18, WPN2_Flashlight_MinAlpha, 255);

  if(!rgb)
    return -1;

  if(!GetEffect("IntFlashbang", pTarget))
  {
    if(!Contained() && rgb)
    {
      var a = rgb->~GetAlpha(), c;
      for(var i = 0; i < GetPlayerCount(); i++)
      {
        var pCursor = GetCursor(GetPlayerByIndex(i))->~GetRealCursor();
        if(!pCursor && !(pCursor = GetCursor(GetPlayerByIndex(i)))) 
          continue;

        if(Contained(pCursor))
          continue;

        var srgb = GetScreenRGB(GetPlayerByIndex(i), SR4K_LayerLight, pCursor);
        var val;

        if(srgb)
          val = srgb->~GetAlpha();

        if(val && 255-a >= val)
          val = 255 - val;
        else
          val = 255 - a;

        var flag = 0;
        if(c != 0)
          flag = MSG_Multiple;

        CustomMessage(Format("<c %x>{{SM07}}</c>", RGBa(255,255,255,BoundBy(val, 1, 254))), pTarget, GetPlayerByIndex(i), 0, 0, 0, 0, 0, flag); 
        c++;
        }
      }
      else
        Message("@", pTarget); 
    }
  return true;
}

public func FxFlashlightBlindnessRefresh(object pTarget, int iNr)
{
  EffectVar(1, pTarget, iNr) = 6;
  return true;
}

public func FxFlashlightBlindnessStop(object pTarget, int iNr)
{
  if(EffectVar(0, pTarget, iNr))
    RemoveObject(EffectVar(0, pTarget, iNr));

  return true;
}

/* Laserpointer */

static const WPN2_ATLaserpointer_Range = 800;	//Laserreichweite

func FxLaserDotTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Nutzer festlegen
  var user = this->~GetUser();

  //Situation prüfen
  var x, y, z;
  if(!user || !user->~IsClonk() || !user->WeaponAt(x, y, z) || Contents(0, user) != this)
  {
    if(pBeam) RemoveObject(pBeam);
    if(pLaser) RemoveObject(pLaser);
    return;
  }

  if(iAttachment == AT_Laserpointer)
  {
    var iAngle = EffectVar(1, user, GetEffect("ShowWeapon", user));
    var fStart = false;
    if(!pLaser)
      pLaser = CreateObject(LRDT,0,0,GetOwner(GetUser(this)));

    var xPos, yPos;
    user->WeaponEnd(xPos,yPos);
    xPos += GetX();
    yPos += GetY();

    var x = GetX(), y = GetY(), xdir = Sin(iAngle, 30), ydir = Cos(iAngle, -30);
    var gravity = GetGravity();

    SetGravity(0);
    
    if(!SimFlight(x, y, xdir, ydir, 0, 0, WPN2_ATLaserpointer_Range/3))
    {
    	x = Sin(iAngle, WPN2_ATLaserpointer_Range) + xPos;
    	y = -Cos(iAngle, WPN2_ATLaserpointer_Range) + yPos;
      pLaser->Stop();
    }
    else if(!pLaser->Active())
      fStart = true;
    
    SetGravity(gravity);

    xdir = Sin(iAngle, 3000);
    ydir = Cos(iAngle, -3000);

    var pEnemy;
    pEnemy = FindObject2(Find_OnLine(0, 0, x - xPos, y - yPos), Find_Hostile(GetOwner(GetUser(this))), Find_NoContainer(), Find_Or(Find_OCF(OCF_Alive), Find_Func("IsBulletTarget", GetID(), this, this), Find_Func("IsCMCVehicle")), Sort_Distance(0, 0));
    
    //Feinderkennung
    if(pEnemy)
    {
      if(!pLaser->Active())
        fStart = true;

      x = GetX(pEnemy);
      y = GetY(pEnemy);

      var xOff, yOff;

      if(xPos > x)
        xOff = GetDefCoreVal("Width", "DefCore", GetID(pEnemy)) + GetDefCoreVal("Offset", "DefCore", GetID(pEnemy), 0) + x;
      else
        xOff = GetDefCoreVal("Offset", "DefCore", GetID(pEnemy), 0) + x;

      if(yPos > y)
        yOff = GetDefCoreVal("Height", "DefCore", GetID(pEnemy)) + GetDefCoreVal("Offset", "DefCore", GetID(pEnemy), 1) + y;
      else
        yOff = GetDefCoreVal("Offset", "DefCore", GetID(pEnemy), 1) + y;

      if(Inside(xPos, Min(x, xOff), Max(x, xOff)))
      {
        x = Sin(iAngle, (yOff - yPos) * 1000 / (-Cos(iAngle, 1000))) + xPos;
        y = -Cos(iAngle, (yOff - yPos) * 1000 / (-Cos(iAngle, 1000))) + yPos;
      }
      else if(Inside(yPos, Min(y, yOff), Max(y, yOff)))
      {
        x = Sin(iAngle, (xOff - xPos) * 1000 / (Sin(iAngle, 1000))) + xPos;
        y = -Cos(iAngle, (xOff - xPos) * 1000 / (Sin(iAngle, 1000))) + yPos;
      }
      else if((Angle(xPos, yPos, xOff, yOff) >= 180 && Angle(xPos, yPos, xOff, yOff) < iAngle) || (Angle(xPos, yPos, xOff, yOff) <= 180 && Angle(xPos, yPos, xOff, yOff) > iAngle))
      {
        x = Sin(iAngle, (yOff - yPos) * 1000 / (-Cos(iAngle, 1000))) + xPos;
        y = -Cos(iAngle, (yOff - yPos) * 1000 / (-Cos(iAngle, 1000))) + yPos;
      }
      else
      {
        x = Sin(iAngle, (xOff - xPos) * 1000 / (Sin(iAngle, 1000))) + xPos;
        y = -Cos(iAngle, (xOff - xPos) * 1000 / (Sin(iAngle, 1000))) + yPos;
      }
  	}

    //Laser zeichnen
    if(!pBeam)  
      pBeam = CreateObject(LRBM, 0, 0, GetOwner(GetUser(this)));
    else
      pBeam->SetPosition(xPos, yPos);

    //Sichtbarkeit nur für Besitzer und Verbündete
    pBeam->SetVisibility(VIS_Owner | VIS_Allies);

    //Laser passend strecken
    pBeam->SetObjDrawTransform(100 * Distance(xPos, yPos, x, y), 0, -450 * Distance(xPos, yPos, x, y), 0, 1000, 0);
    pBeam->SetR(iAngle+90);
    SetPosition(x, y, pLaser);

    if(fStart)
      pLaser->Start();
  }
}

func FxLaserDotStop()
{
  if(pBeam) RemoveObject(pBeam);
  if(pLaser) RemoveObject(pLaser);
}

/* Schalldämpfer */

//EffektVar 0: Alphawert
//EffektVar 1: X-Position
//EffektVar 2: Y-Position
//EffektVar 3: Clonk

func FxSilencerTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Clonk verschwunden: Neuen suchen und Waffentarnung entfernen
  if(!EffectVar(3, pTarget, iEffectNumber))
  {
    var pContainer = Contained(pTarget);
    if(pContainer && Contents(0, pContainer) == pTarget)
      EffectVar(3, pTarget, iEffectNumber) = pContainer;

    if(EffectVar(0, pTarget, iEffectNumber))
    {
      EffectVar(0, pTarget, iEffectNumber) = 0;
      SetClrModulation(RGBa(255, 255, 255, 0), pTarget);
    }
    return;
  }
  //Nicht oder nicht vorne im Inventar: Tarnung bei Waffe und Clonk entfernen, Clonk vergessen
  else if(Contents(0, EffectVar(3, pTarget, iEffectNumber)) != pTarget)
  {
    EffectVar(0, pTarget, iEffectNumber) = 0;
    SetClrModulation(RGBa(255, 255, 255, EffectVar(0, pTarget, iEffectNumber)), EffectVar(3, pTarget, iEffectNumber));
    SetClrModulation(RGBa(255, 255, 255, 0), pTarget);
    EffectVar(3, pTarget, iEffectNumber) = 0;
    return;
  }

  var Alpha = EffectVar(0, pTarget, iEffectNumber);

  //Clonk in Bewegung: Tarnung abschwächen
  if(EffectVar(1, pTarget, iEffectNumber) != GetX(pTarget) || EffectVar(2, pTarget, iEffectNumber) != GetY(pTarget))
  {
    EffectVar(1, pTarget, iEffectNumber) = GetX(pTarget);
    EffectVar(2, pTarget, iEffectNumber) = GetY(pTarget);

    if(EffectVar(0, pTarget, iEffectNumber) >= 4)
      EffectVar(0, pTarget, iEffectNumber) -= 4;
  }
  //Sonst: Tarnung erhöhen
  else if(EffectVar(0, pTarget, iEffectNumber) < 200)
    EffectVar(0, pTarget, iEffectNumber) += 2;
    
  //Clonk reitet oder ist markiert? Sichtbar machen
  if(EffectVar(3, pTarget, iEffectNumber)->~IsRiding() || FindObject2(Find_ID(SM08), Find_ActionTarget(EffectVar(3, pTarget, iEffectNumber)), Find_Action("Attach")))
    EffectVar(0, pTarget, iEffectNumber) = 0;

  //Tarnung verändert: Neuen Wert setzen
  if(Alpha != EffectVar(0, pTarget, iEffectNumber))
    SetClrModulation(RGBa(255, 255, 255, EffectVar(0, pTarget, iEffectNumber)), EffectVar(3, pTarget, iEffectNumber));
}

func FxSilencerStop(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(EffectVar(3, pTarget, iEffectNumber))
    SetClrModulation(RGBa(255, 255, 255, 0), EffectVar(3, pTarget, iEffectNumber));
  if(pTarget)
    SetClrModulation(RGBa(255, 255, 255, 0), pTarget);
}

global func FxShowWeaponUpdate(object pTarget, int iNumber, int iTime)
{
  //Waffe aktualisieren:
  var xoff, yoff, r;  //Offset, Winkel
  var dodraw = false;
  //kein Inventar oder falsche Aktion
  if(!Contents(0,pTarget))
    return EffectCall(pTarget, iNumber, "Reset");
  //Die Waffe momentan überhaupt anzeigen?
  if(!(pTarget->~WeaponAt(xoff, yoff, r)))
    return EffectCall(pTarget, iNumber, "Reset");
  var obj = Contents(0,pTarget), id=GetID(obj);
  //Waffe nicht mehr aktuell
  if(obj && EffectVar(6, pTarget, iNumber) != obj)
  {
    //Neues Objekt ist Waffe, oder ein Objekt, das gezeichnet werden soll
    if(obj->~IsWeapon() || obj->~IsDrawable())
    {
      dodraw = true;
      EffectVar(0, pTarget, iNumber) = id;
      EffectVar(6, pTarget, iNumber) = obj;
      SetGraphics(0, pTarget,id, WeaponDrawLayer, GFXOV_MODE_Object,0,GFX_BLIT_Parent,obj);
    }
    //neues Objekt ist keine Waffe
    else
      return EffectCall(pTarget, iNumber, "Reset");
  }

  id = EffectVar(0, pTarget, iNumber);
  obj = EffectVar(6, pTarget, iNumber);

  if(!obj) return -1;

  //Ausrichtung nach Blickrichtung des Clonks
  //Variablen für die Transformation

  var width, height;		//Breiten- und Höhenverzerrung der Waffe
  var xskew, yskew;		//Zerrung der Waffe, wird zur Rotation gebraucht
  var size;			//Größe der Waffe in der Hand: 1000 = 100%
  //Variablen für die Position
  var dir;			//Richtung, in die das Objekt schaut

  //schnell noch Rotation dazurechnen oder so!
  if(GetEffect("StrikeRecharge", obj))
    r += -Max(Sin(GetEffect("StrikeRecharge", obj, 0, 6)*90/(obj->~GetMCData(MC_Recharge)/4),20),0);
  else
    r += obj->~HandR();

  //Variablen mit Werten versehen
  width = height = xskew = yskew = 1;
  size = id->~HandSize();
  if(!size) size = 1000;
  dir  = GetDir()*2-1;
  if(r > 180 || r < -180)
    dir *= -1;
  r *= dir;

  if(GetEffect("Silencer", obj))
    SetClrModulation(RGBa(255, 255, 255, EffectVar(0, obj, GetEffect("Silencer", obj))), obj);

  if (!dodraw && 90*dir+r == EffectVar(1, pTarget, iNumber) && GetAction(pTarget) == EffectVar(8, pTarget, iNumber))
    return;

  var xfact = size * obj->~HandX();	//Attachpunkte dazurechnen
  var yfact = size * obj->~HandY();

  xoff += Cos(r,xfact)/1000 + dir*Sin(r,yfact)/1000;
  yoff -= Cos(r,yfact)/1000 - dir*Sin(r,xfact)/1000;

  if(dir == 1) 
    height = xskew = yskew = -1;

  r = -90*dir-r-90;
  height *= width *= Cos(r, size);
  xskew *= Sin(r, size);
  yskew *= -xskew;
  xoff *= dir;

  //Waffe
  SetObjDrawTransform(1000,xskew,xoff,yskew,1000,yoff, pTarget, WeaponDrawLayer);	//Position
  SetObjDrawTransform(width,xskew,0,yskew,height,0, obj);				//Größe und Rotation

  //Daten
  var w = GetDefCoreVal("Width",0,id)/2;
  var brlx = id->~BarrelXOffset();
  var brly = id->~BarrelYOffset();

  //Abspeichern, damit abrufbar
  r = -r-90;
  var r2 = (Angle(0,0,w-brlx/1000,brly/1000)-90)*dir;
  var dist = Distance(0,0,w*1000-brlx,brly);
  EffectVar(1, pTarget, iNumber) = r;
  EffectVar(2, pTarget, iNumber) = xoff-Sin(r,size*w);
  EffectVar(3, pTarget, iNumber) = yoff+Cos(r,size*w);
  EffectVar(4, pTarget, iNumber) = xoff+Sin(r+r2,size*(dist))/1000;
  EffectVar(5, pTarget, iNumber) = yoff-Cos(r+r2,size*(dist))/1000;
  EffectVar(7, pTarget, iNumber) = GetDir(pTarget);
  EffectVar(8, pTarget, iNumber) = GetAction(pTarget);
}

global func SALaunchBullet(int iX, int iY, int iOwner, int iAngle, int iSpeed, int iDist, int iDmg, int iRemoveTime, id idType, bool fNoTrail)
{
  //ID des Projektils identifizieren
  var ammoid = idType;
  //Standard nutzen wenn nicht gefunden
  if(!ammoid) ammoid = SHTX;

  var iSize = 2;
  var iTrail = iDmg*10;
  if(fNoTrail) iTrail = -1;

  var iAttachment = 0;
  if(this)
    iAttachment = LocalN("iAttachment", this);

  //Projektil erstellen
  var ammo = CreateObject(ammoid, iX, iY, iOwner);
  ammo->CustomLaunch(iAngle, iSpeed, iDist, iSize, iTrail, iDmg, iRemoveTime, iAttachment);

  return ammo;
}

/*----- Werfen -----*/

public func ControlThrow(caller)
{
  //Nutzer ist Schütze
  SetUser(caller);

  var fWait, meleeattacked;
  if(Contained())
    for (var obj in FindObjects(Find_Container(Contained())))
    {
      if(!obj->~IsWeapon2())
        continue;
      if(GetEffect("StrikeRecharge", obj))
      {
        fWait = true;
        break;
      }
    }

  if(!fWait && GetMCData(MC_CanStrike) && (caller->~ReadyToFire() || caller->~ReadyToAttack()) && !caller->~IsAiming() && (GetFMData(FM_Aim) == 0 || GetUser()->~IsAiming() || GetUser()->~AimOverride()))
  {
    var dir = GetDir(GetUser());
    //Ziele finden
    var obj = FindObjects(Find_InRect(-15+10*dir,-10,20,20), Find_Or(Find_OCF(OCF_Alive), Find_Func("IsMeleeTarget", this)), Find_NoContainer(), Find_Exclude(caller));
    for(var target in obj)
    {
      if(GetOCF(target) & OCF_Alive)
      {
        //Ziel feindlich?
        if(target && (Hostile(GetOwner(GetUser()), GetOwner(target)) || GetOwner(target) == NO_OWNER)) //Hier bewusst kein CheckEnemy, da wir auf FF-Checks verzichten
        {
          //Ziel am kriechen?
          if(target->~IsCrawling())
          {
            //Erhöhten Schaden verursachen
            DoDmg(GetMCData(MC_Damage)*3/2,DMG_Melee,target,0,GetController(GetUser())+1,GetID(), GetAttachment());
            //Ziel zum Aufstehen zwingen
            ObjectSetAction(target, "KneelUp");
          }
          else
          {
            //Schaden verursachen
            DoDmg(GetMCData(MC_Damage),DMG_Melee,target,0,GetController(GetUser())+1,GetID(), GetAttachment());
            SetCommand(GetUser(), "");
            SetComDir(COMD_None, GetUser());

            //Ziel schleudern
            var pwr = GetMCData(MC_Power), angle = GetMCData(MC_Angle);
            if(GetProcedure(target) != "SWIM")
            {
              if(!dir)
                dir--;
              SetXDir(Sin(angle*dir,pwr),target,10);
              SetYDir(-Cos(angle*dir,pwr),target,10);
              ObjectSetAction(target, "Tumble");
            }
          }

          //Achievement-Fortschritt (Fist of Fury)
          if(GetOCF(target) & OCF_CrewMember)
            DoAchievementProgress(1, AC36, GetOwner(GetUser())); 

          if(GetOwner(target) != NO_OWNER && Hostile(GetOwner(target), GetController(GetUser())))
            if(!GetAlive(target) || IsFakeDeath(target))
            {
              //Achievement-Fortschritt (Eat this!)
              DoAchievementProgress(1, AC14, GetOwner(GetUser()));
              //Ribbon-Fortschritt (The Tuna)
              AttemptAwardRibbon(RB07, GetOwner(GetUser()), GetOwner(target));
            }
          meleeattacked = true;
        }
      }
      else
      {
        target->~MeleeHit(this);
        meleeattacked = true;

        //Achievement-Fortschritt (Fly Swatter)
        if(target && target->~IsMAV() && target->~IsDestroyed() && (Abs(GetXDir(target)) + Abs(GetYDir(target)) >= 25))
          DoAchievementProgress(1, AC42, GetOwner(GetUser()));
      }
    }
  }
  if (meleeattacked)
  {
    //Sound
    Sound("ClonkMelee*.ogg", 0, this);
    Sound("WPN2_Punch*.ogg", 0, this);
    if(iAttachment == AT_Bayonet)
      if(GetOCF(target) & OCF_Alive)
        Sound("BKHK_RotorHit*.ogg", 0, this);
      else
        Sound("WPN2_BayonetHit.ogg", 0, this);

    //Cooldown
    AddEffect("StrikeRecharge", this, 1, 1, this);
  }

  //Automatischen Schuss beenden, wenn erneut Werfen gedrückt
  if(IsRecharging())
  {
    if(!GetPlrCoreJumpAndRunControl(GetController(caller)))
    {
      StopAutoFire();
      if(GetFMData(FM_Auto) && !fWait && !meleeattacked) OnFireStop(firemode);
    }
    return 1;
  }

  //Kein Feuer erlauben wenn Kolbenschlag ausgeführt
  if(meleeattacked || fWait)
    return 1;

  //Unterstützt der Schussmodus das zielen aber es wird nicht gezielt?
  if(GetFMData(FM_Aim)>0 && !(GetUser()->~IsAiming()) && !(GetUser()->~AimOverride()))
  {
    //Bereit zum Zielen?
    if(GetUser()->~ReadyToAim())
      //Zielen starten
      GetUser()->StartAiming();

    //Nachladen?
    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    //Nachladen wenn möglich sofern Munition verbraucht
    if(!CheckAmmo(ammoid,ammousage,this))
      if(CheckAmmo(ammoid,ammousage,GetUser()))
      {
        Reload();
      }
      //Nicht genügend Munition
      else
      {
        //Unmöglichkeit des Nachladens angeben
        PlayerMessage(GetOwner(caller), "$NotEnoughAmmo$", caller, ammoid);
      }
    return 1;
  }

  //Feuern! Fehlgeschlagen?
  if(!Fire())
  {
    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    //Nachladen wenn möglich sofern Munition verbraucht
    if(!CheckAmmo(ammoid,ammousage,this))
      if(CheckAmmo(ammoid,ammousage,GetUser()))
      {
        Reload();
      }
      //Nicht genügend Munition
      else
      {
        //Unmöglichkeit des Nachladens angeben
        PlayerMessage(GetOwner(caller), "$NotEnoughAmmo$", caller, ammoid);
        Sound("WPN2_Empty.ogg");
      }
  }
  else
  {
    if(GetFMData(FM_BurstAmount) > 1 && !GetEffect("BurstFire", this))
    {
      var rechargetime = GetFMData(FM_BurstRecharge);
      if(rechargetime)
        AddEffect("BurstFire", this, 1, rechargetime, this, 0, GetFMData(FM_BurstAmount));
      else
      {
        for(var i = GetFMData(FM_BurstAmount); i > 0; i--)
          if(!Fire())
            return 1;
      }
    }
  }

  return 1;
}