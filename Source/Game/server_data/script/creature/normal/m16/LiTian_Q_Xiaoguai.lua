--������������
function m16_xiaoguai_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local XiaoGuai_Num = map.GetMapScriptData(MapID,InstanceID,1,1)
	map.SetMapScriptData(MapID,InstanceID,1,1,XiaoGuai_Num+1)

	if XiaoGuai_Num+1< 24 then

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1709)	--���ѻ�ɱxx����������ɱ24������޽�����֣�
		msg.AddMsgEvent(MsgID, 9, XiaoGuai_Num+1)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

	elseif XiaoGuai_Num+1== 24 then

		map.MapCreateColCreature(MapID, InstanceID, 1005601, 1915, 5303, 1759, 1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1710)	--�����Ѿ�����������[1915, 1759]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

	elseif XiaoGuai_Num+1< 40 then

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1711)	--���ѻ�ɱxx����������ɱ40������㽫����֣�
		msg.AddMsgEvent(MsgID, 9, XiaoGuai_Num+1)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

	elseif XiaoGuai_Num+1== 40 then

		map.MapCreateColCreature(MapID, InstanceID, 1005435, 1978, 5303, 1609, 1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1712)	--�����Ѿ�����������[1978, 1609]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

	elseif XiaoGuai_Num+1< 85 then

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1713)	--���ѻ�ɱxx����������ɱ85����Ѫ�ݽ�����֣�
		msg.AddMsgEvent(MsgID, 9, XiaoGuai_Num+1)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

	elseif XiaoGuai_Num+1== 85 then

		map.MapCreateColCreature(MapID, InstanceID, 1005487, 1726, 3078, 1478, 1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1714)	--Ѫ���Ѿ�����������[1726, 1478]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

	elseif XiaoGuai_Num+1< 150 then

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1715)	--���ѻ�ɱxx����������ɱ150�������꽫����֣�
		msg.AddMsgEvent(MsgID, 9, XiaoGuai_Num+1)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

	elseif XiaoGuai_Num+1== 150 then

		map.MapCreateColCreature(MapID, InstanceID, 1005602, 1478, 1503, 1950, 1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1716)	--�����Ѿ�����������[1478, 1950]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

	elseif XiaoGuai_Num+1< 200 then

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1717)	--���ѻ�ɱxx����������ɱ200��������ս�������Ž�����֣�
		msg.AddMsgEvent(MsgID, 9, XiaoGuai_Num+1)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

	elseif XiaoGuai_Num+1== 200 then

		map.MapCreateColCreature(MapID, InstanceID, 1005433, 1330, 3265, 2399, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1005434, 1434, 3265, 2357, 1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1718)	--����ս�������˵������Ѿ����֣�
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

	end
end

aux.RegisterCreatureEvent(1005421, 4, "m16_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1005422, 4, "m16_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1005423, 4, "m16_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1005425, 4, "m16_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1005426, 4, "m16_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1005427, 4, "m16_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1005428, 4, "m16_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1005429, 4, "m16_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1005430, 4, "m16_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1005431, 4, "m16_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1005479, 4, "m16_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1005480, 4, "m16_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1005481, 4, "m16_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1005482, 4, "m16_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1005483, 4, "m16_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1005484, 4, "m16_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1005485, 4, "m16_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1005486, 4, "m16_xiaoguai_OnDie")
