/*-- M203-Splittergranate --*/

#strict 2
#include M203

func ExplodeDelay() { return 80; }

func Trigger()
{
  for(var i = 20; i > 0; --i)
  {
    var angle = Random(360);//Interpolate4K(Random(360),Angle(GetXDir(),GetYDir()),0,80,BoundBy(Distance(GetXDir(),GetYDir()),0,80)) - 180;
    var ammo = CreateObject(SHRP,0,0,GetController());
    ammo->Launch(angle,70+Random(30),100+Random(100),5,15,15);
  }
  
  CreateParticle("Blast",0,0,0,0,10*15,RGB(255,255,128));//FakeBlast :°
  Sound("Blast1");
  Sparks(15,RGB(255,128));
  CastParticles("Smoke3",6,8,0,0,80,150,RGBa(255,255,255,120),RGBa(255,255,255,150));
  AddLightFlash(50,0,0,RGB(255,255,128),this);
  RemoveObject();
}
