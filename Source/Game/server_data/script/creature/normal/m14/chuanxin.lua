--������������
function m14_chuanxin_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local Chuanxin_Num = map.GetMapScriptData(MapID,InstanceID,1,8)
	map.SetMapScriptData(MapID,InstanceID,1,8,Chuanxin_Num+1)

	if Chuanxin_Num+1== 40 then
		map.MapCreateCreature(MapID, InstanceID, 1004529, 473, 150, 903)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1733)	--����ӳ������Ѿ�����������[473, 903]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	elseif Chuanxin_Num+1< 40 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1734)	--���ѻ�ɱxx�����Ĳ��ڣ���ɱ40��������ӳ����Ľ�����֣�
		msg.AddMsgEvent(MsgID, 9, Chuanxin_Num+1)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end
end

aux.RegisterCreatureEvent(1004506, 4, "m14_chuanxin_OnDie")
aux.RegisterCreatureEvent(1004508, 4, "m14_chuanxin_OnDie")
aux.RegisterCreatureEvent(1004510, 4, "m14_chuanxin_OnDie")
