/*-- Waffenscript-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto WPN2

local iAttachment, pLaser, pBeam;

static const AT_NoAttachment	= 0;	//Kein Waffenaufsatz
static const AT_ExtendedMag	= 1;	//Erweitertes Magazin
static const AT_Bayonet		= 2;	//Bajonett
static const AT_Laser		= 3;	//Laserpointer
static const AT_Silencer	= 4;	//Schalldämpfer
static const AT_Foregrip	= 5;	//Frontgriff


/* Waffenaufsatz festlegen */

func SetAttachment(int iValue)
{
  if(GetEffect("LaserDot", this)) RemoveEffect("LaserDot", this);

  iAttachment = iValue;
  if(iAttachment == AT_Laser) AddEffect("LaserDot", this, 1, 1, this);
  
  if(Contained()) Contained()->~UpdateCharge();
}

/* Laserpointer */

func FxLaserDotTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Nutzer festlegen
  var user = this->~GetUser();
  var x, y, z;
  if(!user || !user->~IsClonk() || !user->WeaponAt(x, y, z) || Contents(0, user) != this)
  {
    if(pBeam) RemoveObject(pBeam);
    if(pLaser) RemoveObject(pLaser);
    return;
  }

  if(iAttachment == AT_Laser)
  {
    var iAngle = EffectVar(1, user, GetEffect("ShowWeapon", user));
    var fStart = false;
    if(!pLaser)
      pLaser = CreateObject(LRDT,0,0,GetOwner(this));

    var xPos, yPos;
    user->WeaponEnd(xPos,yPos);
    xPos += GetX();
    yPos += GetY();

    var x = GetX(), y = GetY(), xdir = Sin(iAngle, 30), ydir = Cos(iAngle, -30);
    var gravity = GetGravity();

    SetGravity(0);
    if (!SimFlight(x, y, xdir, ydir))
      pLaser->Stop();
    else 
      if(!pLaser->Active())
        fStart = true;
    SetGravity(gravity);

    xdir = Sin(iAngle, 3000);
    ydir = Cos(iAngle, -3000);

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
      else
        if(Inside(yPos, Min(y, yOff), Max(y, yOff)))
        {
          x = Sin(iAngle, (xOff - xPos) * 1000 / (Sin(iAngle, 1000))) + xPos;
          y = -Cos(iAngle, (xOff - xPos) * 1000 / (Sin(iAngle, 1000))) + yPos;
        }
      else
        if((Angle(xPos, yPos, xOff, yOff) >= 180 && Angle(xPos, yPos, xOff, yOff) < iAngle) || (Angle(xPos, yPos, xOff, yOff) <= 180 && Angle(xPos, yPos, xOff, yOff) > iAngle))
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
    pBeam = CreateObject(LRBM, 0, 0, GetOwner(this));
  else
    pBeam->SetPosition(xPos, yPos);

  //Sichtbarkeit nur für Besitzer und Verbündete
  pBeam->SetVisibility(VIS_Owner | VIS_Allies);

  //Laser passend strecken
  if(pEnemy || pLaser->Active())
    pBeam->SetObjDrawTransform(100 * Distance(xPos, yPos, x, y), 0, -450 * Distance(xPos, yPos, x, y), 0, 1000, 0);
  else
    pBeam->SetObjDrawTransform(100 * Distance(xPos, yPos, xPos + xdir/3, yPos + ydir/3), 0, -450 * Distance(xPos, yPos, xPos + xdir/3, yPos + ydir/3), 0, 1000, 0);

  pBeam->SetR(iAngle+90);
  SetPosition(x, y, pLaser);
  if(fStart) pLaser->Start();
  }
}