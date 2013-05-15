/*-- Blendgranate --*/

#strict 2

#appendto STUN

public func FxIntFlashbangStart(object pTarget, int iEffectNumber, int iTemp, intensity, owner)
{
  if(iTemp)
    return;

  //Keine Intensität?
  if(!intensity) return -1;

  EffectVar(0,pTarget,iEffectNumber) = intensity;
  EffectVar(2,pTarget,iEffectNumber) = owner;	//Zur Punkteberechnung

  //Blendung stark genug? Dann auch hörbar
  if(intensity > 38) Sound("STUN_Bang.ogg", false, pTarget, 100, GetOwner(pTarget)+1);

  //Stärke berechnen und Blendung erstellen
  var a = BoundBy(255-(intensity*255/100),0,255);

  //Ziel bereits geblendet? Hinzuaddieren
  /*if(GetEffectCount("IntFlashbang", pTarget) > 1)
  {
    for(var i = 0; i < GetEffectCount("IntFlashbang", pTarget); i++)
    {
       var nr = GetEffect("IntFlashbang", pTarget, i);
      if(nr != iEffectNumber)
      {
      	Log("INTENSITY: %d, ALPHA-ADD: %d, SRGB: %v, SRGB-ALPHA: %d", intensity, a, GetScreenRGB(GetOwner(pTarget), SR4K_LayerLight, pTarget), GetScreenRGB(GetOwner(pTarget), SR4K_LayerLight, pTarget)->GetAlpha());
        GetScreenRGB(GetOwner(pTarget), SR4K_LayerLight, pTarget)->DoAlpha(a, 0, 255);
        //EffectVar(0, pTarget, nr) = BoundBy(EffectVar(0, pTarget, nr) + intensity, 0, 255);
        return -1;
      }
    }
  }*/
  var srgb = GetScreenRGB(GetOwner(pTarget), SR4K_LayerLight, pTarget);
  if(srgb)
  {
  	srgb->~DoAlpha(255-a, 0, 255);
  	
  	if(GetEffectCount("IntFlashbang", pTarget) > 1)
  		return -1;
  }
 
  var flash = ScreenRGB(pTarget,RGBa(255,255,255,a), 0, 0, false, SR4K_LayerLight);
  if(!flash)
    return -1;
  else
    EffectVar(1,pTarget,iEffectNumber) = flash;
}

public func FxIntFlashbangTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	var rgb = EffectVar(1,pTarget,iEffectNumber);
	if(!rgb) return -1;
  //var i = EffectVar(0,pTarget,iEffectNumber)--;

	//Aktuelle Intensität anhand des ScreenRGB-Alphawerts ermitteln
  var i = ((255-rgb->~GetAlpha()) * 100 / 255)-1;

  if(i <= 0) return -1;
  
  var a = BoundBy(255-(i*255/100),0,255);
  rgb->SetAlpha(a);
  
  var val, num, pCursor, c, flag;
  if(!Contained())
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