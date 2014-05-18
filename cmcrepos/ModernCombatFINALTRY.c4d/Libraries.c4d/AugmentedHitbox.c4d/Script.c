/*-- Erweiterte Hitbox --*/

#strict 2

local iHitboxDistance, aHitboxAngles;


/* Hitbox */

public func HitboxXOffset()	{return 0;}	//X-Abstand vom Offset zum Hitboxmittelpunkt
public func HitboxYOffset()	{return 0;}	//Y-Abstand vom Offset zum Hitboxmittelpunkt
public func HitboxWidth()	{return 10;}	//Breite der Hitbox
public func HitboxHeight()	{return 10;}	//Höhe der Hitbox
public func HitboxRotation()	{return 0;}	//Hitbox rotieren (zusätzlich zur Objektrotation)
public func UseOwnHitbox()	{return true;}

public func BulletHitboxFactor(int ax, int ay, int bx, int by, int cx, int cy, int dx, int dy)
{
  return((cx-ax)*(dy-cy)-(cy-ay)*(dx-cx))*1000/((bx-ax)*(dy-cy)-(by-ay)*(dx-cx));
}

local aBulHitboxParameter, iLastBulHitboxCheckFrm, fBulHitboxResult;

public func BulletHitboxStretch(int bul_start_x, int bul_start_y, int bul_end_x, int bul_end_y)
{
  var par = aBulHitboxParameter;
  if(fBulHitboxResult && aBulHitboxParameter && (iLastBulHitboxCheckFrm == FrameCounter()))
    if(par[0] == bul_start_x && par[1] == bul_start_y && par[2] == bul_end_x && par[3] == bul_end_y && par[4] == GetX() && par[5] == GetY() && par[6] == GetR())
      return [par[7], par[8]];

  return [-1];
}

public func BulletHitboxCheck(int bul_start_x, int bul_start_y, int bul_end_x, int bul_end_y)
{
  if(!iHitboxDistance || !aHitboxAngles)
    return false;

  //Find_Func ruft pro Frame gegebenenfalls mehrfach auf: Keine neuen Berechnungen
  var par = aBulHitboxParameter;
  if(aBulHitboxParameter && (iLastBulHitboxCheckFrm == FrameCounter()))
    if(par[0] == bul_start_x && par[1] == bul_start_y && par[2] == bul_end_x && par[3] == bul_end_y && par[4] == GetX() && par[5] == GetY() && par[6] == GetR())
      return fBulHitboxResult;

  aBulHitboxParameter = [bul_start_x, bul_start_y, bul_end_x, bul_end_y, GetX(), GetY(), GetR()];
  iLastBulHitboxCheckFrm = FrameCounter();

  bul_start_x = AbsX(bul_start_x); 
  bul_start_y = AbsY(bul_start_y); 
  bul_end_x   = AbsX(bul_end_x);
  bul_end_y   = AbsY(bul_end_y);

  var hitbox = GetHitboxPoints();
  var length = GetLength(hitbox);

  var x1 = hitbox[length-1][0];
  var y1 = hitbox[length-1][1];
  var x2 = hitbox[0][0];
  var y2 = hitbox[0][1];

  fBulHitboxResult = false;
  aBulHitboxParameter[8] = 0;
  for(var i = 0; i <= length-1; i++)
  {
    var fac1 = BulletHitboxFactor(bul_start_x, bul_start_y, bul_end_x, bul_end_y, x1, y1, x2, y2);
    var fac2 = BulletHitboxFactor(x1, y1, x2, y2, bul_start_x, bul_start_y, bul_end_x, bul_end_y);

    if(Inside(fac1, 0, 1000) && Inside(fac2, 0, 1000))
    {
      if(!aBulHitboxParameter[7] || fac1 < aBulHitboxParameter[7])
        aBulHitboxParameter[7] = fac1;
        aBulHitboxParameter[8]++;
      fBulHitboxResult = true;
    }

    if(i >= length-1)
      break;

    x1 = hitbox[i][0];
    y1 = hitbox[i][1];
    x2 = hitbox[i+1][0];
    y2 = hitbox[i+1][1];
  }

  return fBulHitboxResult;
}

public func GetHitboxPoints()
{
  var distance = iHitboxDistance;
  var angle = aHitboxAngles;
  var hitbox = [];
  for(var a in angle)
  {
    a += GetR()+HitboxRotation();
    a = a + (45 - a) * 2;
    hitbox[GetLength(hitbox)] = [Cos(a, distance)+HitboxXOffset(), -Sin(a, distance)+HitboxYOffset()];
  }

  return hitbox;
}

public func IsInHitbox(int x, int y, bool fDraw)
{
  if(!iHitboxDistance || !aHitboxAngles)
    return false;

  var hitbox = GetHitboxPoints();

  if(fDraw)
  {
    var lastpoint = 0;
    for(var point in hitbox)
    {
      if(!lastpoint)
      {
        lastpoint = point;
        continue;
      }

      DrawParticleLine("PSpark", lastpoint[0], lastpoint[1], point[0], point[1], 5, 30, RGB(255), RGB(255));
      lastpoint = point;
    }

    DrawParticleLine("PSpark", lastpoint[0], lastpoint[1], hitbox[0][0], hitbox[0][1], 5, 30, RGB(255), RGB(255));
    return true;
  }

  var inside = false;

  var length = GetLength(hitbox);

  var x1 = hitbox[length-1][0];
  var y1 = hitbox[length-1][1];
  var x2 = hitbox[0][0];
  var y2 = hitbox[0][1];

  var su = (y1 >= y);
  for(var i = 1; i <= length; i++)
  {
    var eu = (y2 >= y);
    if(su != eu && (x1 > x || x2 > x))
    {
      if((y2 - y) * (x2 - x1) <= (y2 - y1) * (x2 - x))
      {
        if(eu)
          inside = !inside;
      }
      else
        if(!eu)
          inside = !inside;
    }

    if(i >= length)
      break;

    su = eu;
    y1 = y2;
    x1 = x2;
    x2 = hitbox[i][0];
    y2 = hitbox[i][1];
  }

  return inside;
}

public func ExplosionHitbox(int iEX, int iEY, int iER)
{
	if(!iHitboxDistance || !aHitboxAngles)
    return false;
  
  var hitbox = GetHitboxPoints();
  
 	iEX = AbsX(iEX); iEY = AbsY(iEY);
 	Log("ExplosionHitbox-Check: iEX: %d, iEY: %d, iER: %d, hitbox: %v", iEX, iEY, iER, hitbox);

	for(var i = 0; i < GetLength(hitbox); i++)
	{
		Log("i: %d, i+1: %d, lgt: %d", i, (i+1) % GetLength(hitbox), GetLength(hitbox));
		var p1 = hitbox[i], p2 = hitbox[(i+1) % (GetLength(hitbox))];
		var v1 = [p2[0]-p1[0], p2[1]-p1[1]], v2 = [p1[0]-iEX,p1[1]-iEY];
		var t = (v1[0]*v2[0]+v1[1]*v2[1])*1000/Sqrt(v1[0]**2+v1[1]**2)**2;
		
		if(t > 1000)
			t = 1000;
		else if(t < 0)
			t = 0;
		
		var vd = [p1[0]+(v1[0]*t)/1000, p1[1]+(v1[1]*t)/1000];
		
		if(Distance(iEX, iEY, vd[0], vd[1]) < iER)
			return true;
		
		DrawParticleLine("PSpark", iEX, iEY, vd[0], vd[1], 10, 50, RGB(255), RGB(255));
	}
	
	return false;
}

public func SwitchHitboxVisibility()
{
  var effect = GetEffect("ShowHitbox", this);
  if(!effect)
    AddEffect("ShowHitbox", this, 101, 10, this);
  else
    EffectVar(0, this, effect) = !EffectVar(0, this, effect);

  return true;
}

public func FxShowHitboxTimer(object target, int nr)
{
  if(!EffectVar(0, target, nr))
    target->IsInHitbox(0, 0, true);

  return true;
}

protected func InitializeHitbox()
{
  //Hitbox erstellen
  iHitboxDistance = Distance(0, 0, HitboxWidth()/2, HitboxHeight()/2);
  aHitboxAngles = [Angle(0, 0, HitboxWidth()/2, HitboxHeight()/2), Angle(0, 0, -(HitboxWidth()/2), HitboxHeight()/2), Angle(0, 0, -(HitboxWidth()/2), -(HitboxHeight()/2)), Angle(0, 0, HitboxWidth()/2, -(HitboxHeight()/2))];

  return true;
}
	
