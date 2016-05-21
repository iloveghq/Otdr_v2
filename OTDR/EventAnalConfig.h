#pragma once

struct EventAnalConfig
{
	float BendTH;//衰减门限
	float ReflectTH; //反射门限
	float Termial; //光纤末端
	float BackScatter;//后向散射因子 1310nm
	float BackScatter1550;//后向散射因子 1550nm //zll 2012.10.16
	float BackScatter1625;//后向散射因子 1550nm //zll 2013.5.10 晚
	float Refraction;//折射率 1310nm
	float Refraction1550;//折射率 1550nm //zll 2012.10.16
	float Refraction1625;//折射率 1625nm //zll 2013.5.10 晚

	float Reflect; 
};