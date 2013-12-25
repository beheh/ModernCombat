/*-- Schuss --*/

#strict 2

local iTime,lx,ly,pTrail,iDamage,speed,max_dst,dst,fb,fNoTrail,iMaxHits,iHitReduction;
local shooter,wpnid,iAttachment;					//Schütze, Waffe und Waffenaufsatz

func NoWarp()			{return true;}
func IsBullet()			{return true;}
public func AllowHitboxCheck()	{return false;}


/* Initialisierung */

public func Initialize()
{
  SetObjectBlitMode(1);
}

func Construction(object byObj)
{
  //Kein Objekt: Verschwinden
  if(!byObj)
    return;

  //ID der Waffe setzen
  wpnid = GetID(byObj);

  //Schütze setzen
  shooter = GetShooter(byObj);

  //Team setzen
  if(shooter->GetTeam())
    SetTeam(shooter->GetTeam());
}

/* Kugel-Abschuss (langsame Kugeln) */

public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iRemoveTime, int iMultiHit, int iMultiHitReduce)
{
  return LaunchFB(iAngle,iSpeed,iDist,iSize,iTrail,iDmg,iRemoveTime,iMultiHit,iMultiHitReduce,...);
}

public func LaunchFB(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iRemoveTime, int iMultiHit, int iMultiHitReduce)
{
  //Schaden der Kugel setzen
  if(!iDmg)
    iDamage = 3;
  else
    iDamage = iDmg;

  if(iMultiHit > 1)
    iMaxHits = iMultiHit;
  else
    iMaxHits = 1;

  iHitReduction = iMultiHitReduce;

  //Position setzen
  SetPosition(GetX(),GetY()+GetDefHeight()/2);
  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetR(iAngle);
  DoCon(100*iSize/GetDefWidth()-100);

  SetAction("Travel");

  max_dst = iDist;
  speed = iSpeed;
  iTime = 1;
  fb = true;

  HitCheck(iAngle,iDist);

  Remove(iRemoveTime);
}

public func SetAngle(int iAngle, int iAPrec)
{
  if(!iAPrec) iAPrec = 1;
  SetXDir(+Sin(iAngle,speed,iAPrec));
  SetYDir(-Cos(iAngle,speed,iAPrec));
  SetR(iAngle/iAPrec,0);
}

private func CreateTrail(int iSize, int iTrail)
{
  if(iTrail < 0)
    return;

  pTrail = CreateObject(TRAI,0,0,-1);
  if(pTrail)
  {
    pTrail->Set(iSize-2,iTrail,this);
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

/* Querschläger */

public func RicochetAngle()	//maximaler Abprallwinkel
{
  if(GetID() != SHTX) return;
  return 50;
}

public func OffsetX(int iA, int iB)
{
  var off = Offset(iA,iB);
  if(off > 180)
  {
    var da = -iA;
    var db = -iB;

    if(iA > 180)
      da = 360-iA;

    if(iB > 180)
      db = 360-iB;

    iA = 180+db;
    iB = 180+da;

    off = Offset(iA,iB);
  }
  return off;
}

public func Offset(int iA, int iB)
{
  if(iA > iB)
    return iA-iB;
  else
    return iB-iA;
}

public func Flip(int iA, int iB)
{
  var off = OffsetX(iA,iB);
  if(iA > iB)
    return iA+off;
  return iA-off;
}

public func Ricochet(int iX, int iY)
{
  if(GBackSolid()) return;
  if(iDamage <= 1) return;

  var I = Wrap4K(GetR(),0,360);
  var A = SurfaceNormal4K(iX,iY,2);
  var O = Wrap4K(Flip(A,I-180),0,360);
  var H = Wrap4K(OffsetX(O,I),0,360);

  /*Log("In:%d] Surface:%d] Abprallwinkel:%d] Reflektionswinkel:%d]",I,A,H,O);

  DrawParticleLine ("PSpark",0,0,Sin(I,100),-Cos(I,100),4,30,RGB(0,255),RGB(0,255));
  DrawParticleLine ("PSpark",0,0,Sin(A,100),-Cos(A,100),2,20,RGB(255),RGB(255));
  DrawParticleLine ("PSpark",0,0,Sin(O,100),-Cos(O,100),4,30,RGB(0,128,255),RGB(0,128,255));
  DrawParticleLine ("PSpark",0,0,Sin(H,100),-Cos(H,100),2,20,RGB(0,128,255),RGB(255,255,255));*/

  if(H <= RicochetAngle())//Winkel okay?
  {
    var O = A+(A-I)+180;
    iDamage = iDamage-iDamage*(H*50/RicochetAngle())/100;//Maximal 50% abziehen.
    Sound("Ricochet*.ogg");

    HitCheck(O,max_dst-dst);

    return true;
  }
}

/* Aufprall und Treffer */

private func HitLandscape()
{
  //Koordinaten bestimmen
  var x,y;
  x = lx;
  y = ly;

  //Der eigentliche Aufprall
  OnHitLandscape(x,y);

  //Umliegende Objekte beschädigen
  var objs = FindObjects(Find_AtPoint(), Find_NoContainer(), Find_Or(Find_Func("IsBulletTarget",GetID(),this,shooter), Find_OCF(OCF_Alive)), Find_Func("CheckEnemy",this), Find_Not(Find_Func(ObjectCall(this,"HitExclude"))));  
  {
    for(var pTarget in objs)
      HitObject(pTarget);
  }

  //Verschwinden
  HitObject();
}

public func OnHitLandscape(int iX, int iY)
{
  var rgb = 0;
  var tmp = CreateObject(TIM1,iX,iY,-1);	//Sound-Objekt

  //Aufprall auf Material?
  if(GBackSolid(iX,iY))
  {
    //Material und dessen Farbe bestimmen
    var mat = GetMaterial(iX,iY);
    var rand = Random(3);
    rgb = RGB(GetMaterialColor(mat,rand,0),
    		GetMaterialColor(mat,rand,1),
    		GetMaterialColor(mat,rand,2));

    //Material grabbar?
    if(GetMaterialVal("DigFree", "Material", mat))
    {
      //Material weggraben und versprühen
      CastPXS(MaterialName(mat), 20, 40, iX, iY);
      DigFree(GetX()+iX, GetY()+iY, 5);

      //Sounds
      Sound("BulletHitSoft*.ogg", 0, tmp);
      Sound("Crumble*.ogg", 0, tmp);
    }
    else
    {
      //Sounds
      Sound("BulletHit*.ogg", 0, tmp);
      Sound("Crumble*.ogg", 0, tmp);
    }
  }
  else
  {
    //Sounds
    Sound("BulletHit*.ogg", 0, tmp);
    Sound("Crumble*.ogg", 0, tmp);
  }

  //Aufpralleffekte
  if(rgb)
  {
    var alphamod, sizemod;
    CalcLight(alphamod, sizemod);
    SmokeBurst((100*GetCon()/100)*sizemod/100,iX,iY,GetR()-180,0,rgb);
  }

  //Sound-Objekt entfernen
  RemoveObject(tmp);
}

public func HitObject(object pObject)
{
  //Schütze kann sich nicht selbst treffen
  if(shooter && pObject)
    if(pObject == shooter)
      return false;

  //Objekt treffen
  if(BulletStrike(pObject))
  {
    var x,y;
    x = lx;
    y = ly;
    OnBulletHit(pObject,x,y);
    return true;
  }
}

public func OnBulletHit(object pObject, int iX, int iY)
{
  //Zu treffendes Objekt vorhanden?
  if(pObject)
  {
    //Soundeffekt
    var tmp = CreateObject(TIM1, iX, iY, NO_OWNER);
    Sound("BulletImpact*.ogg", 0, tmp);
    RemoveObject(tmp);
  }

  //Lebewesen erhalten keine zusätzlichen Effekte
  if(GetOCF(pObject) & OCF_Living)
    return;

  //Partikel verschleudern
  var iAngle, iSpeed, iSize;

  for( var i = 0; i < 3 * GetCon() / 10; ++i)
  {
    iAngle = GetR() - 200 + Random(41);
    iSpeed = Random(speed / 9 + 1);
    iSize = Random(31) + 10;
    CreateParticle("Frazzle", iX + Sin(iAngle, 2), iY - Cos(iAngle, 2), Sin(iAngle, iSpeed), -Cos(iAngle, iSpeed), iSize, GlowColor(iTime));
  }

  var alphamod, sizemod, iColor = GlowColor(iTime);
  CalcLight(alphamod, sizemod);

  var r, g, b, a;
  SplitRGBaValue(iColor, r, g, b, a);
  iColor = RGBa(r, g, b, Min(a + alphamod, 255));

  if(GetEffectData(EFSM_BulletEffects) > 0)
    CreateParticle("Flare", iX, iY, Random(201)- 100, Random(201) - 100, GetCon() * sizemod / 20, iColor);
}

public func BulletStrike(object pObj)
{
  if(pObj)
    DoDmg(iDamage, DMG_Projectile, pObj, 0, 0, 0, iAttachment);
  return true;
}

private func Color(int iATime)
{
  var iPrg = 100*iATime/iTime;
  return RGBa(255,255-iPrg*2,255-iPrg*2,iPrg*2);
}

public func TrailColor(int iATime)
{
  var iPrg = 100*iATime/iTime;
  return RGBa(255,255-iPrg*2,255-iPrg*2,iPrg*2);
}

public func GlowColor(int iATime)
{
  return RGBa(255,190,0,50);
}

func HitExclude()	{}

public func Remove(int iRemoveTime)
{
  if(pTrail)
  {
    SetPosition(GetX(),GetY(),pTrail);
    pTrail->Remove();
  }

  if(!GetEffect("Remove",this))
  {
    BulletParticle();
    //SetVisibility(VIS_None);
    SetXDir();
    SetYDir();
    SetClrModulation(RGBa(255,255,255,255));
    AddEffect("Remove",this,1,Max(1,iRemoveTime),0,GetID());
  }
}

private func HitCheck(int r, int d)
{
  // calculate length
  var mx = +Sin(r,100),
      my = -Cos(r,100),
      ml = d;
  var sx = GetX(),
      sy = GetY();

  var g = GetGravity();
  SetGravity(0);
  var x = sx, y = sy, xdir = mx, ydir = my, fLandscape;
  fLandscape = SimFlight(x, y, xdir, ydir, 0, 0, d/10);
  
  var liqX = sx, liqY = sy, liqXdir = mx, liqYdir = my, fLiquid;
  fLiquid = SimFlight(liqX, liqY, liqXdir, liqYdir, 25, 49, d/10);
  SetGravity(g);

  if(x == sx && y == sy)
  {
    x += Sin(r, d);
    y -= Cos(r, d);
  }

  mx = AbsX(x);//+Sin(r,ml);
  my = AbsY(y);//-Cos(r,ml);
  
  var aExcludes = [2, [5, shooter], [5, this]];

  for(var i = 0; i < iMaxHits; i++)
  {
    var pObj = FindObject2(Find_OnLine(0, 0, mx, my), 
    Find_NoContainer(), 
    Find_Or(Find_Func("IsBulletTarget", GetID(), this, shooter), Find_OCF(OCF_Alive)),
    Find_Func("CheckEnemy", this),
    Find_Not(Find_Func("HitExclude", this)),
    aExcludes,
    Find_Or(Find_Not(Find_Func("UseOwnHitbox")), Find_Func("BulletHitboxCheck", sx, sy, x, y)),
    Sort_Distance());
    if(pObj)
    {
      var stretch;
      if(pObj->~UseOwnHitbox() && (stretch = pObj->BulletHitboxStretch(sx, sy, x, y))[0] > 0 && stretch[1] != 4)
      {
        if(stretch[1] == 1)
        {
          lx = 0;
          ly = 0;
        }
        else
        {
          var dist = Distance(sx, sy, x, y);
          lx = Sin(r, (dist * stretch[0]) / 1000);
          ly = -Cos(r, (dist * stretch[0]) / 1000);
        }
      }
      else
      {
        //Bei eigener Hitbox eigenen Bulletcheck ausführen (z.B. bei Helikoptern)
        var ox = GetX(pObj), oy = GetY(pObj);

        //Objektwerte verwenden statt Definitionswerte (bspw. für Assaulttargets)
        if(pObj->~BulletCheckObjectHitbox())
        {
          var xLeft = GetObjectVal("Offset", 0, pObj, 0) + ox;
          var xRight = GetObjectVal("Width", 0, pObj) + GetObjectVal("Offset", 0, pObj, 0) + ox;

          var yUp = GetObjectVal("Offset", 0, pObj, 1) + oy;
          var yDown = GetObjectVal("Height", 0, pObj) + GetObjectVal("Offset", 0, pObj, 1) + oy;
        }
        else if(pObj->~UseOwnHitbox())
        {
          var xLeft = -pObj->HitboxWidth()/2 + ox + 25*pObj->~IsSmoking();
          var xRight = pObj->HitboxWidth()/2 + ox - 25*pObj->~IsSmoking();

          var yUp = -pObj->HitboxHeight()/2 + oy + 25*pObj->~IsSmoking();
          var yDown = pObj->HitboxHeight()/2 + oy - 25*pObj->~IsSmoking();
        }
        else
        {
          var xLeft = GetDefCoreVal("Offset", "DefCore", GetID(pObj), 0) + ox;
          var xRight = GetDefCoreVal("Width", "DefCore", GetID(pObj)) + GetDefCoreVal("Offset", "DefCore", GetID(pObj), 0) + ox;

          var yUp = GetDefCoreVal("Offset", "DefCore", GetID(pObj), 1) + oy;
          var yDown = GetDefCoreVal("Height", "DefCore", GetID(pObj)) + GetDefCoreVal("Offset", "DefCore", GetID(pObj), 1) + oy;
        }

        if(!(Inside(sx, xLeft, xRight) && Inside(sy, yUp, yDown)))
        {
          var xOff, yOff;

          if(sx > ox)
            xOff = xRight;
          else
            xOff = xLeft;

          if(sy > oy)
            yOff = yDown;
          else
            yOff = yUp;

          var a = Angle(sx, sy, xOff, yOff);
          if(Inside(sx, Min(ox, xOff), Max(ox, xOff)))
          {
            var temp = (yOff - sy) * 1000 / (-Cos(r, 1000));
            lx = Sin(r, temp);
            ly = -Cos(r, temp);
          }
          else if(Inside(sy, Min(oy, yOff), Max(oy, yOff)))
          {
            var temp = (xOff - sx) * 1000 / (Sin(r, 1000));
            lx = Sin(r, temp);
            ly = -Cos(r, temp);
          }
          else
          {
            var temp = (yOff - sy) * 1000 / (-Cos(r, 1000));
            var lxOne = Sin(r, temp);
            var lyOne = -Cos(r, temp);

            temp = (xOff - sx) * 1000 / (Sin(r, 1000));
            var lxTwo = Sin(r, temp);
            var lyTwo = -Cos(r, temp);

            //Wenn erster Punkt weiter weg, diesen wählen, sonst den anderen
            if(Distance(lxOne, lyOne) > Distance(lxTwo, lyTwo))
            {
              lx = lxOne;
              ly = lyOne;
            }
            else
            {
              lx = lxTwo;
              ly = lyTwo;
            }
          }
        }
      }
      var fAlive = GetOCF(pObj) & OCF_Alive;
      if(HitObject(pObj))
      {
        var dist = Distance(sx, sy, ox, oy);
        dst += dist;
        if(!fAlive || i == (iMaxHits - 1))
        {
          if(fLiquid && Distance(sx, sy, lx, ly) > Distance(sx, sy, liqX, liqY)+5)
            HitLiquid(sx, sy, liqX, liqY);

          Remove();
          return dist;
        }
        else
        {
          aExcludes[GetLength(aExcludes)] = [5, pObj];
          //Log("Before %d", iDamage);
          iDamage = (iDamage * (100 - iHitReduction)) / 100;
          //Log("After %d", iDamage);
        }
      }
    }
  }

  lx = mx;
  ly = my;
  
  if(fLiquid)
    HitLiquid(sx, sy, liqX, liqY);

  if(fLandscape)//Nicht in der Luft. :O
    HitLandscape(mx,my);

  ml = Distance(sx, sy, mx, my);

  dst += ml;
  return ml;
}

public func HitLiquid(int iStartX, int iStartY, int iLiqX, int iLiqY)
{
  var angle = Angle(iStartX, iStartY, iLiqX, iLiqY);
  var x = AbsX(iLiqX), y = AbsY(iLiqY);
  var temp = CreateObject(TIM1, x, y, NO_OWNER);

  CreateParticle("MuzzleFlash4", x, y, +Sin(angle, 500), -Cos(angle, 500), 180, 0, temp);
  Splash(x, y+1, 15);
  Sound("BulletHitWater*.ogg", false, temp, 50);

  for(var i = 0; i < 5; i++)
  {
    var bubble = CreateObject(FXU1, x, y+2, NO_OWNER);
    if(!bubble)
      break;

    bubble->SetXDir(+Sin(angle+Random(30)-15, 100));
    bubble->SetYDir(-Cos(angle+Random(30)-15, 100));
  }
}

private func NotZero(int a)
{
  if(!a) return 1;
  else return a;
}

public func FxRemoveTimer(object target, int effect, int time)
{
  RemoveObject(target);
  return -1;
}

public func GetKillIcon()
{
  return wpnid;
}

public func BulletParticle()
{
  if(fNoTrail) return;
  //if(!Random(25-Min(GetCon()/4,25))) return;

  var l = Distance(lx,ly)-20;//-hinten
  if(l < 40) return;//hinten + vorne
  //if(Random((200-Min(l,200))) < 100) return;

  var p = 20+Random(l-20);
  var s = 30+Random(10);

  CreateParticle("BulletTail",lx*p/l,ly*p/l,
                             +Sin(GetR(),s),
                             -Cos(GetR(),s),
                             (200*GetCon()/100)*5,GlowColor(iTime));
}

global func GetShooter(object weapon)
{
  var shooter;

  if(!weapon) return;

  //Waffe?
  if(weapon->~IsWeapon())
  {
    shooter = weapon->GetUser();
    //Schütze ist irgendwo drin
    if(Contained(shooter))
      shooter = Contained(shooter);
    //oder fasst was an
    if(shooter)
      if(GetActionTarget(0, shooter) && (GetProcedure(shooter) == "PUSH"))
        shooter = GetActionTarget();
  }

  //Wenn immernoch kein Schütze identifiziert, stattdessen Waffe nehmen
  if(!shooter)
    shooter = weapon;
      return shooter;
}

/* SpecialAmmo-System */

//public func IsSpecialAmmo()	{return SHTX == GetID();}
public func IsSpecialAmmo()	{return true;}

public func CustomLaunch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iRemoveTime, int Attachment, int iMultiHit, int iMultiHitReduce)
{
  fNoTrail = (iTrail == -1);
  iAttachment = Attachment;
  return Launch(iAngle,iSpeed,iDist,iSize,iTrail,iDmg,iRemoveTime, iMultiHit, iMultiHitReduce);
}

public func CustomBulletCasing(int iX, int iY, int iXDir, int iYDir, int iSize, int iColor)
{
  return BulletCasing(iX,iY,iXDir,iYDir,iSize,iColor);
}

public func FMMod(int iType,Data)
{
  return Data;
}
