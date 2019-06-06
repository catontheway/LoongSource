function m107_xiaoguai_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local i = role.GetRoleItemNum(RoleID, 3400101)
	local j = role.GetRoleItemNum(RoleID, 1350051)+role.GetRoleItemNum(RoleID, 1350052)+role.GetRoleItemNum(RoleID, 1350053)+role.GetRoleItemNum(RoleID, 1350054)

	if i~=0 and j~=0 then

		local killnum = role.GetRoleScriptData(RoleID, 1, RoleDataType["m107_killnum"])
		local lastkill = role.GetRoleScriptData(RoleID, 1, RoleDataType["m107_lastkill"])

		local curday = tonumber(os.date("%j"))

		if curday~=lastkill then

			killnum = 0
			role.SetRoleScriptData(RoleID, 1, RoleDataType["m107_killnum"], 0)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["m107_lastkill"], curday)

		end

		killnum=killnum+1

		role.SetRoleScriptData(RoleID, 1, RoleDataType["m107_killnum"], killnum)

		if killnum<=3000 and (killnum%300~=0 or killnum==0) then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 3620)	--�ѻ�ɱС��xֻ������ɱ1000ֻ�ɽ������ı�����Ϊ�������
			msg.AddMsgEvent(MsgID, 9, killnum)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		elseif killnum<=3000 and killnum%300==0 then

			for k=1350051, 1350054 do

				if 0==role.RemoveFromRole(MapID, InstanceID, RoleID, k, 1, 420) then
					break
				end

			end

			role.AddRoleItem(MapID, InstanceID, RoleID, 3400102, 1, -1, 8, 420)

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 3621)	--�ѻ�ɱС��1000ֻ���ɹ���һ�������ı�����Ϊ�������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		elseif killnum>3000 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 3622)	--�������������10������������������޷��ٽ�������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		end

	elseif i~=0 and j==0 then

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 3623)	--��������������ӵ�������ı�ʱ�ſ�ͨ����ɱ����������ʹ����������¯����������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	end

end

aux.RegisterCreatureEvent(1543503, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543504, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543505, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543506, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543507, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543508, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543509, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543510, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543511, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543512, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543513, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543514, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543515, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543516, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543517, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543518, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543519, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543520, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543521, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543522, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543523, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543524, 4, "m107_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1543525, 4, "m107_xiaoguai_OnDie")
