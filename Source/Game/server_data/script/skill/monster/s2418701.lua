
----ʹ�ü����ٻ�����ű�

--а��������1010004��ʹ�ü��� ��2418701�� ���ٻ�4������1010011

function s2418701_Cast(MapID, InstanceID, SkillID, OwnerID)
    local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	map.MapCreateCreature(MapID, InstanceID, 1010011, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1010011, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1010011, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1010011, x, y, z)
	return 0
end


--ע��

aux.RegisterSkillEvent(2418701, 1, "s2418701_Cast")

