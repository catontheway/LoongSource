
--������������
function c1535326_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local k = math.random(100)
	if k == 77 then
	    if role.GetBagFreeSize(RoleID) > 1 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4000023, 1, -1, 8, 420)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 3131)    --XXX����˶Գ����������һ��������˴�˵�е�������������ޣ�
			msg.DispatchWorldMsgEvent(MsgID)
		end
	end

end

aux.RegisterCreatureEvent(1535326, 4, "c1535326_OnDie")

