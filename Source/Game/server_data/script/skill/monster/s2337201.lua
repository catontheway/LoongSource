
--ʹ�ü����ٻ�����ű�

--��1290112��ʹ�ü��� ��2337201�� ���ٻ�1������1290120��3������1290121


function s2337201_Cast(MapID, InstanceID, SkillID, OwnerID)
    local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	map.MapCreateCreature(MapID, InstanceID, 1290120, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1290121, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1290121, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1290121, x, y, z)
	return 0
end


--ע��

aux.RegisterSkillEvent(2337201, 1, "s2337201_Cast")
