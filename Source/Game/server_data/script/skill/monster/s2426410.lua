
----ʹ�ü����ٻ�����ű�

--�����ʹ��1006522��ʹ�ü��� ��2426410�� ���ٻ�2������1006536

function s2426410_Cast(MapID, InstanceID, SkillID, OwnerID)
    local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	map.MapCreateCreature(MapID, InstanceID, 1006536, x-4, y, z-4)
	map.MapCreateCreature(MapID, InstanceID, 1006536, x+4, y, z+4)
	return 0
end


--ע��

aux.RegisterSkillEvent(2426410, 1, "s2426410_Cast")
