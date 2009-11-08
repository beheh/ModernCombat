/* Kab000m! 2.0 */

#strict 2

global func Explode(int iLevel, object pObj, id idEffect, string szEffect)
{
  if(!pObj)
    if(!(pObj=this))
      return;
  
  var x = AbsX(pObj->GetX()),
  	  y = AbsY(pObj->GetY());
			
  var boom = FindObject(BOOM);
  if(!boom) boom = CreateObject(BOOM,0,0,-1);
			
	var xdir = GetXDir(pObj) + GetWind(GetX(pObj),GetY(pObj))/20;
	var ydir = GetYDir(pObj);
	
	var opt_angle = Angle(xdir,ydir);
	var speed = BoundBy(Distance(xdir,ydir),0,60);
  
	
	///Feuer-, Funken- und Dirt-Effekte...
  var i=0, count = 3+iLevel/8, angle = Random(360);
  while((count > 0) && (++i < count*10))
  {
    angle += RandomX(40,80);
    
    angle = Interpolate4K(angle,opt_angle,0,60,speed);
    angle -= 180;
        
    //Rauch
    var smokex = +Sin(angle,RandomX(iLevel/4,iLevel/2));
    var smokey = -Cos(angle,RandomX(iLevel/4,iLevel/2));
    if(GBackSolid(x+smokex,y+smokey))
      continue;
     var level = iLevel + Random(iLevel/5);

		var a = angle+RandomX(-15,+15);
		for(var i = 7; i > 0; i--)
		  CreateParticle("BlastSpark1", smokex, smokey, +Sin(a,level+RandomX(-5,+5)), -Cos(a,level+RandomX(-5,+5)), 25+Random(50)); 
    CreateParticle("BlastFlame", smokex, smokey, +Sin(angle,level/2), -Cos(angle,level/2), level*5);
    for(var i = Random(3)+1; i > 0; i--)
		{
		  a = angle+RandomX(-30,+30);
		  CreateParticle("BlastDirt", smokex, smokey, +Sin(a,level+RandomX(-20,+20)), -Cos(a,level+RandomX(-20,+20)), level*RandomX(7,12));
		}
		//CreateSmokeTrail(level,angle,smokex,smokey,pObj);
    count--;
  }

	///Brandspuren...
  if(EffectLevel() > EM4K_Low)
    CreateBurnMark(x,y,iLevel,20+iLevel/2);

  ///Lichteffekte bei Dunkelheit...
  if(IsDark())
  {
    var iSize = iLevel*100;
    if(iLevel < 20)
      iSize /= 2;
    AddLightFlash(iSize/3, x, y, RGBa(255,220,64,15));
  }
	
	///Feuer-Effekt..
	/*angle = Interpolate4K(0,opt_angle,0,120,speed);
	angle -= 180;
	CreateParticle("BlastBg",0,0,+Sin(angle,100),-Cos(angle,100),iLevel*20);
	//CreateParticle("BlastBg",0,0,0,-1,iLevel*20);
	*/
	
	///Der eigentliche Blast-Partikel...
	CreateParticle("Blast",x,y,0,0,iLevel*11);

	///Standart-Verhalten...
  return inherited(iLevel, pObj, idEffect, szEffect);
}

global func SemiExplode(int iLevel, int incidence)
{
  for(var i = 0; i < incidence; i++)
  {
    var rock = CreateObject(ROCK, 0, 0, GetOwner());
    if(this)
    {
      rock->SetXDir(GetXDir(this));
      rock->SetYDir(GetYDir(this));
    }
    Explode(iLevel,rock); 
  }
  RemoveObject();
  return 1;
}

global func CreateBurnMark(int iX, int iY, int iLevel, int Count) 
{
  var boom;
  if(!ObjectCount(BOOM)) boom = CreateObject(BOOM,0,0,-1);
  else boom = FindObject(BOOM);
  
  var angle=Random(360/Count); //variablen für die überprüfung
  var type;
  for(var z; z < Count; z++)
	{
    angle += Max(1,360/Count);

    // Check: Sky or Solid/Liquid
    var x = iX+Cos(angle,iLevel);
    var y = iY+Sin(angle,iLevel);
    if((GetMaterialVal("Density","Material",GetMaterial(x,y)) != 0) || (GetMaterial(x,y) == -1))
      continue;

    type = 1+Random(3);
    var size = BurnMarkCheck(angle,RandomX(iLevel/2,iLevel*2),iX,iY);
    var sin = Sin(angle,(size-iLevel)/2+iLevel+Random(3));
    var cos = Cos(angle,(size-iLevel)/2+iLevel+Random(3));

    CreateParticle("BurnMark",iX+cos,iY+sin,Cos(angle+RandomX(-5,5),50),Sin(angle+RandomX(-5,5),50),size*5+Random(25),RGBa(0,0,0,64),boom,1); 
  }
}

global func BurnMarkCheck(int angle,int size, int iX, int iY)
{
  var sin = Sin(angle,size);
  var cos = Cos(angle,size);
  var x = cos, y = sin, i=100;

  while(GetMaterial(iX+x,iY+y) == -1 || GetMaterialVal("Density","Material",GetMaterial(iX+x,iY+y)) != 0)
	{
    x = cos*i/100;
    y = sin*i/100;
    if(i <= 0) return 0;
    i-=10;
  }

  return size;
}

global func CreateSmokeTrail(int iStrength, int iAngle, int iX, int iY, object pObj)
{
  iX += GetX(pObj);
  iY += GetY(pObj);
  AddEffect("SmokeTrail", 0, 300, 1, 0, 0, iStrength, iAngle, iX, iY);
}

// Variablen:
// 0 - Stärke
// 1 - momentane Stärke
// 2 - X-Position
// 3 - Y-Position
// 4 - Anfangs-X-Geschwindigkeit
// 5 - Anfangs-Y-Geschwindigkeit
global func FxSmokeTrailStart(object pTarget, int iEffectNumber, int iTemp, iStrength, iAngle, iX, iY)
{
  if(iTemp)
    return;
  
  if(iAngle%90 == 1) iAngle += 1;
  iStrength = Max(iStrength,5);

  EffectVar(0, pTarget, iEffectNumber) = iStrength;
  EffectVar(1, pTarget, iEffectNumber) = iStrength;
  EffectVar(2, pTarget, iEffectNumber) = iX;
  EffectVar(3, pTarget, iEffectNumber) = iY;
  EffectVar(4, pTarget, iEffectNumber) = +Sin(iAngle,iStrength*40);
  EffectVar(5, pTarget, iEffectNumber) = -Cos(iAngle,iStrength*40);
}

global func FxSmokeTrailTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var iStrength = EffectVar(0, pTarget, iEffectNumber);
  var iAStr = EffectVar(1, pTarget, iEffectNumber);
  var iX = EffectVar(2, pTarget, iEffectNumber);
  var iY = EffectVar(3, pTarget, iEffectNumber);
  var iXDir = EffectVar(4, pTarget, iEffectNumber);
  var iYDir = EffectVar(5, pTarget, iEffectNumber);

  iAStr = Max(1,iAStr-iAStr/10+Random(2));
  iAStr--;
  iYDir += GetGravity()*2/3;
  iYDir += GetWind(iX,iY)/20;

  var xdir = iXDir*iAStr/iStrength;
  var ydir = iYDir*iAStr/iStrength;

  // Neu: Random
  iX += RandomX(-3,3);
  iY += RandomX(-3,3);
  
  // zuerst zeichnen
  CreateParticle("Smoke3",iX,iY,RandomX(-2,2),RandomX(-2,2)-3,20+iAStr*8,RGBa(128,128,128,55+35*iAStr/iStrength));
  CreateParticle("Blast",iX,iY,0,0,10+iAStr*8,RGBa(250,100+Random(100),100,160));

  // dann nächste position berechnen
  iX += xdir/100;
  iY += ydir/100;
  
  if(GBackSemiSolid(iX,iY))
    return -1;
  if(iAStr <= 1)
    return -1;
    
  EffectVar(1, pTarget, iEffectNumber) = iAStr;
  EffectVar(2, pTarget, iEffectNumber) = iX;
  EffectVar(3, pTarget, iEffectNumber) = iY;
  EffectVar(5, pTarget, iEffectNumber) = iYDir;
}

global func FxSmokeTrailStop()
{
  //...
}