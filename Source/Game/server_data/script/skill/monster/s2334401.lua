
--ʹ�ü����ٻ�����ű�

--���� ��1290101�� ʹ�ü��� 2334401  �ٻ�����(1003134,1003142)


function s2334401_Cast(MapID, InstanceID, SkillID, OwnerID)
    local x, y, z = unit.GetPosition(MapID, InstanceID, OwnerID)
	map.MapCreateCreature(MapID, InstanceID, 1003134, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1003142, x, y, z)
	return 0
end


--ע��

aux.RegisterSkillEvent(2334401, 1, "s2334401_Cast")
