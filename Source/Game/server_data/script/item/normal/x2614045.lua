--ս������
--�������ϲ���[����̨][x,y,z]��[��תԴ][x,y,z]��[����ݵ�][x,y,z]��[�����ָ���][x,y,z]����ʹ����Ʒ3֧[ħ��Ž�][2614045],��33%�����ٻ���[�������][1007042][߳������][1007043][��Ӥ����][1007044]
--����̨��ͼ����objIDΪ 7929(��ͼ���Ϊm06_1)
--��תԴ��ͼ����objIDΪ 7930(��ͼ���Ϊm06_1)
--����ݵ���ͼ����objIDΪ 7931(��ͼ���Ϊm06_1)
--�����ָ�����ͼ����objIDΪ 7932(��ͼ���Ϊm06_1)

function I2614045_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	--ɾ��2֧[ħ��Ž�][2614045]
	role.RemoveFromRole(MapID, InstanceID, TargetID, 2614045, 2, 101)

	local Rate = math.random(1, 100)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)

	--33%�����ٻ���[�������][1007042]
	if Rate < 34 then
	map.MapCreateCreature(MapID, InstanceID, 1007042, x+2, y, z+2)

	--33%�����ٻ���[߳������][1007043]
	elseif Rate >=34 and Rate < 67 then
	map.MapCreateCreature(MapID, InstanceID, 1007043, x+2, y, z+2)

	--33%�����ٻ���[��Ӥ����][1007044]
	elseif Rate>= 67 and Rate <= 100 then
	map.MapCreateCreature(MapID, InstanceID, 1007044, x+2, y, z+2)
	end
end

function I2614045_CanUse(MapID, InstanceID, TypeID, TargetID)
	local num = role.GetRoleItemNum(TargetID, 2614045)
	local bRet, bIgnore = 0, false
	local a = map.IsInArea(1146339967,InstanceID, TargetID, 7929)
	local b = map.IsInArea(1146339967,InstanceID, TargetID, 7930)
	local c = map.IsInArea(1146339967,InstanceID, TargetID, 7931)
	local d = map.IsInArea(1146339967,InstanceID, TargetID, 7932)

	if num < 3 then
		bRet = 32
	else
		if (a == false) and (b == false) and (c == false) and (d == false) then
		bRet = 43
		end
	end
	return bRet, bIgnore

end

aux.RegisterItemEvent(2614045, 1, "I2614045_QuestUsable")
aux.RegisterItemEvent(2614045, 0, "I2614045_CanUse")
