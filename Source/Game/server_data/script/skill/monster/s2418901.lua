
----ʹ�ü����ٻ�����ű�

--а�ķ�����1010051��ʹ�ü��� ��2418901�� ���ٻ�4������1010052

function s2418901_Cast(MapID, InstanceID, SkillID, OwnerID)
    local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	map.MapCreateCreature(MapID, InstanceID, 1010052, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1010052, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1010052, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1010052, x, y, z)
	return 0
end


--ע��

aux.RegisterSkillEvent(2418901, 1, "s2418901_Cast")
