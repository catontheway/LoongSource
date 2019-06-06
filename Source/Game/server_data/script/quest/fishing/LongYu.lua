--[[
����

��Ϸ��	2000��	100%	�ض����

����������һ����
���ʯ	1	20%
����Ǭ��ͼ��	1	15%
�������ͼ��	1	15%
��������ͼ��	1	15%
���ɿ�ˮͼ��	1	15%
�������ͼ��	1	15%
ս�����䣨1Сʱ��	1	5%


�ƽ�����

��Ϸ��	1��	100%	�ض����

����������һ����
[��]������	5	30%
[��]̫��ͼ	5	30%
[��]����ӡ	5	30%
���ʯ	5	30%
������	1	5%
��ͨ�ϳɷ�?Ҽ	1	3%
��ͨ�ϳɷ�?��	1	1%
ս�����䣨8Сʱ��	1	1%



	]]


function I3200712_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local a = math.random(1000)
	local Temp = 0

	local FishingLevel = role.GetRoleScriptData(TargetID, 1, RoleDataType["FishingLevel"])
	if FishingLevel > 100 then
		FishingLevel = 100
	end
	if FishingLevel < 0 then
		FishingLevel = 1
	end
	role.SetRoleScriptData(TargetID, 1, RoleDataType["FishingLevel"],FishingLevel)

	role.AddRoleSilver(MapID, InstanceID, TargetID, 2000, 102)

	if a <= 200 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3200464, 1, -1, 8, 420)
	elseif a <= 350 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2005003, 1, -1, 8, 420)
	elseif a <= 500 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2005023, 1, -1, 8, 420)
	elseif a <= 650 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2005033, 1, -1, 8, 420)
	elseif a <= 800 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2005043, 1, -1, 8, 420)
	elseif a <= 950 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2005053, 1, -1, 8, 420)
	elseif a <=1000 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 1360123, 1, -1, 8, 420)
		Temp= 1360123
	end

	if Temp ~= 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100111)
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 4, 3200712)
		msg.AddMsgEvent(MsgID, 4, Temp)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end
end

function I3200712_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
	return 40, false
	end
	return 0, false
end

aux.RegisterItemEvent(3200712, 1, "I3200712_GiftBag")
aux.RegisterItemEvent(3200712, 0, "I3200712_CanUseGiftBag")


function I3200713_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local a = math.random(1000)
	local Temp = 0

	local FishingLevel = role.GetRoleScriptData(TargetID, 1, RoleDataType["FishingLevel"])
	if FishingLevel > 100 then
		FishingLevel = 100
	end
	if FishingLevel < 0 then
		FishingLevel = 1
	end
	role.SetRoleScriptData(TargetID, 1, RoleDataType["FishingLevel"],FishingLevel)

	role.AddRoleSilver(MapID, InstanceID, TargetID, 10000, 102)

	if a <= 300 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2615304, 1, -1, 8, 420)
	elseif a <= 600 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2615306, 1, -1, 8, 420)
	elseif a <= 900 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2615308, 1, -1, 8, 420)
	elseif a <= 950 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3304001, 1, -1, 8, 420)
		Temp=3304001
	elseif a <= 980 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 1412001, 1, -1, 8, 420)
	elseif a <= 990 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 1412002, 1, -1, 8, 420)
		Temp=1412002
	elseif a <=1000 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 1360122, 1, -1, 8, 420)
		Temp= 1360122
	end

	if Temp ~= 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100111)
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 4, 3200713)
		msg.AddMsgEvent(MsgID, 4, Temp)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end

end


function I3200713_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
	return 40, false
	end
	return 0, false
end

aux.RegisterItemEvent(3200713, 1, "I3200713_GiftBag")
aux.RegisterItemEvent(3200713, 0, "I3200713_CanUseGiftBag")
