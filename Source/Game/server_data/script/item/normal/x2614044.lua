--ս������
--�������ϲ���[����̨][x,y,z]��[��תԴ][x,y,z]��[����ݵ�][x,y,z]��[�����ָ���][x,y,z]������ʹ����Ʒ5֧[ħ�����][2614044],�ٻ���[ħ�彫��][1007041]
--����̨��ͼ����objIDΪ 7929(��ͼ���Ϊm06_1)
--��תԴ��ͼ����objIDΪ 7930(��ͼ���Ϊm06_1)
--����ݵ���ͼ����objIDΪ 7931(��ͼ���Ϊm06_1)
--�����ָ�����ͼ����objIDΪ 7932(��ͼ���Ϊm06_1)




--�������ϲ���[����̨][x,y,z]��[��תԴ][x,y,z]��[����ݵ�][x,y,z]��[�����ָ���][x,y,z]ʹ����Ʒ5֧[ħ�����][2614044],�ٻ���[ħ�彫��][1007041]
function I2614044_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	--ɾ��5֧[ħ�����][2614044]
	role.RemoveFromRole(MapID, InstanceID, TargetID, 2614044, 4, 101)
	--�ٻ���[ħ�彫��][1007041]
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 1007041, x+2, y, z+2)

end


function I2614044_CanUse(MapID, InstanceID, TypeID, TargetID)
	local num = role.GetRoleItemNum(TargetID, 2614044)
	local bRet, bIgnore = 0, false
	local a = map.IsInArea(1146339967, InstanceID, TargetID, 7929)
	local b = map.IsInArea(1146339967, InstanceID, TargetID, 7930)
	local c = map.IsInArea(1146339967, InstanceID, TargetID, 7931)
	local d = map.IsInArea(1146339967, InstanceID, TargetID, 7932)

	if num < 5 then
		bRet = 32
	else
		if (a == false) and (b == false) and (c == false) and (d == false) then
		bRet = 43
		end
	end
	return bRet, bIgnore

end



aux.RegisterItemEvent(2614044, 1, "I2614044_QuestUsable")
aux.RegisterItemEvent(2614044, 0, "I2614044_CanUse")
