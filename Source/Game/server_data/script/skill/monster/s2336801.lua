
--ʹ�ü����ٻ�����ű�

--���棨1290111��ʹ�ü��� ��2336801�� ���ٻ�3ֻ����(1290122)


function s2336801_Cast(MapID, InstanceID, SkillID, OwnerID)
    local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	map.MapCreateCreature(MapID, InstanceID, 1290122, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1290122, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1290122, x, y, z)
	return 0
end


--ע��

aux.RegisterSkillEvent(2336801, 1, "s2336801_Cast")
