--�ɷ�ʹ�ú���
function i3400102_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
	for i = 1, 4 do

		for j = 0,3 do

			if 1 <= role.GetRoleItemNum(TargetID, 3400000+i+j*10) then
				return bRet, bIgnore
			end

		end
	end
	--����
	return 32, bIgnore
end

aux.RegisterItemEvent(3400102, 0, "i3400102_CanUse")

--�Խ�ָ��������
function I3400102_OnUse(MapID, InstanceID, TypeID, RoleID)

	local ZLSQ_lv = 0
	local orginal_id = 0
	local min_id = 0

	for i = 1, 4 do

		for j = 0,3 do

			if 0 == role.RemoveFromRole(MapID, InstanceID, RoleID, 3400000+i+j*10, 1, 420) then
				ZLSQ_lv = j+1
				orginal_id = 3400000+i+j*10
				min_id = 3400000+i
				break
			end
		end

		if ZLSQ_lv~=0 then
			break
		end
	end

	if ZLSQ_lv==1 then

		local r = math.random(10000)

		if r>7000 then --30%ʧ��
			role.AddRoleItem(MapID, InstanceID, RoleID, orginal_id, 1, -1, 8, 420)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 3614)	--����ʧ�ܣ��ȼ����䡣
			msg.AddMsgEvent(MsgID, 4, min_id)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else			--70%�ɹ�
			role.AddRoleItem(MapID, InstanceID, RoleID, orginal_id+10, 1, -1, 8, 420)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 3617)	--�����ɹ����ȼ�+10.
			msg.AddMsgEvent(MsgID, 4, min_id)
			msg.AddMsgEvent(MsgID, 9, (ZLSQ_lv+6)*10)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end

	elseif ZLSQ_lv==2 then

		local have_bdf = 0

		if 0==role.RemoveFromRole(MapID, InstanceID, RoleID, 3400103, 1, 420) then
			have_bdf=1
		end

		local r = math.random(10000)

		if r>3300 then --67%ʧ��
			if have_bdf==0 then
				role.AddRoleItem(MapID, InstanceID, RoleID, orginal_id-10, 1, -1, 8, 420)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 3616)	--����ʧ�ܣ��ȼ�-10��
				msg.AddMsgEvent(MsgID, 4, min_id)
				msg.AddMsgEvent(MsgID, 9, (ZLSQ_lv+4)*10)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, orginal_id, 1, -1, 8, 420)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 3615)	--����ʧ�ܣ��ȼ����䡣
				msg.AddMsgEvent(MsgID, 4, min_id)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else			--33%�ɹ�
			role.AddRoleItem(MapID, InstanceID, RoleID, orginal_id+10, 1, -1, 8, 420)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 3617)	--�����ɹ����ȼ�+10.
			msg.AddMsgEvent(MsgID, 4, min_id)
			msg.AddMsgEvent(MsgID, 9, (ZLSQ_lv+6)*10)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end

	elseif ZLSQ_lv==3 then

		local have_bdf = 0

		if 0==role.RemoveFromRole(MapID, InstanceID, RoleID, 3400103, 1, 420) then
			have_bdf=1
		end

		local r = math.random(10000)

		if r>1600 then --84%ʧ��
			if have_bdf==0 then
				role.AddRoleItem(MapID, InstanceID, RoleID, orginal_id-10, 1, -1, 8, 420)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 3616)	--����ʧ�ܣ��ȼ�-10��
				msg.AddMsgEvent(MsgID, 4, min_id)
				msg.AddMsgEvent(MsgID, 9, (ZLSQ_lv+4)*10)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, orginal_id, 1, -1, 8, 420)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 3615)	--����ʧ�ܣ��ȼ����䡣
				msg.AddMsgEvent(MsgID, 4, min_id)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else			--16%�ɹ�
			role.AddRoleItem(MapID, InstanceID, RoleID, orginal_id+10, 1, -1, 8, 420)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 3617)	--�����ɹ����ȼ�+10.
			msg.AddMsgEvent(MsgID, 4, min_id)
			msg.AddMsgEvent(MsgID, 9, (ZLSQ_lv+6)*10)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end

	elseif ZLSQ_lv==4 then

		local have_bdf = 0

		if 0==role.RemoveFromRole(MapID, InstanceID, RoleID, 3400103, 1, 420) then
			have_bdf=1
		end

		local r = math.random(10000)

		if r>500 then --95%ʧ��
			if have_bdf==0 then
				role.AddRoleItem(MapID, InstanceID, RoleID, orginal_id-10, 1, -1, 8, 420)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 3616)	--����ʧ�ܣ��ȼ�-10��
				msg.AddMsgEvent(MsgID, 4, min_id)
				msg.AddMsgEvent(MsgID, 9, (ZLSQ_lv+4)*10)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, orginal_id, 1, -1, 8, 420)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 3615)	--����ʧ�ܣ��ȼ����䡣
				msg.AddMsgEvent(MsgID, 4, min_id)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else			--5%�ɹ�
			role.AddRoleItem(MapID, InstanceID, RoleID, orginal_id+10, 1, -1, 8, 420)

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 3617)	--�����ɹ����ȼ�+10.
			msg.AddMsgEvent(MsgID, 4, min_id)
			msg.AddMsgEvent(MsgID, 9, (ZLSQ_lv+6)*10)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 3618)	--���xxx������100����������
			msg.AddMsgEvent(MsgID, 2, RoleID)
			msg.AddMsgEvent(MsgID, 4, min_id)
			msg.DispatchWorldMsgEvent(MsgID)
		end

	end

end

aux.RegisterItemEvent(3400102, 1, "I3400102_OnUse")
