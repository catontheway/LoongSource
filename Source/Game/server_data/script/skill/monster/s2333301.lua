--����6  1006715 ����ս����5���ͷż��ܣ�2333301��������λ��ˢ�� 1006716

function s2333301_Cast(MapID, InstanceID, SkillID, RoleID)
         local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
		 map.MapCreateCreature(MapID, InstanceID, 1006716, x, y, z)
end

aux.RegisterSkillEvent(2333301, 1, "s2333301_Cast")
