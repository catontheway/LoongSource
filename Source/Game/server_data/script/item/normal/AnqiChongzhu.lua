	--RoleDataType["ChongzhuYellow"]	= 157			��ɫ����������¼
	--RoleDataType["ChongzhuGreen"]	= 158				��ɫ����������¼
	--RoleDataType["ChongzhuBlue"]		= 159			��ɫ����������¼
	--RoleDataType["ChongzhuOrange"]		= 160		��ɫ����������¼
function I6000224_OnUse(MapID,InstanceID,TypeID,RoleID)
	local ChongzhuYellow = role.GetRoleScriptData(RoleID,1,RoleDataType["ChongzhuYellow"])			--��ȡ��һ�ɫ����������¼
	for yellow = 8700001,8700006 do
	    if role.GetRoleItemNum(RoleID,yellow) > 0 then												--����������л�ɫ����
			local yellowUp = yellow + 6
			local yellowDown = yellow - 6
			local shibaiNum = math.floor(ChongzhuYellow/10000)										--��ȡ��ɫ��������ʧ�ܴ���
			local bubianNum = math.floor(ChongzhuYellow/10)%1000									--��ȡ��ɫ���������������
			local shengjiNum = ChongzhuYellow%10													--��ȡ��ɫ�����Ƿ��ֱ������
			if shengjiNum >= 1 or bubianNum >= 2 then												--���������ֱ���������߲������Ϊ3����
				role.RemoveFromRole(MapID,InstanceID,RoleID,yellow,1,420)							--ɾ�����л�ɫ����
				role.AddRoleItem(MapID, InstanceID, RoleID, yellowUp, 1, 2, 8, 420)					--�����ɫ����
				role.SetRoleScriptData(RoleID,1,RoleDataType["ChongzhuYellow"],0)					--���м�¼��0
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 0)
				msg.AddMsgEvent(MsgID, 1, 4304)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif	bubianNum < 2 and shengjiNum == 0 then											--����������С��3
				role.RemoveFromRole(MapID, InstanceID, RoleID, yellow, 1, 420)						--ɾ�����л�ɫ����
				role.AddRoleItem(MapID, InstanceID, RoleID, yellow, 1, 1, 8, 420)					--��ӻ�ɫ����
				role.SetRoleScriptData(RoleID,1,RoleDataType["ChongzhuYellow"],ChongzhuYellow+10)	--��ɫ�����������������1
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 0)
				msg.AddMsgEvent(MsgID, 1, 4302)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
			return
		end
	end

	local ChongzhuGreen = role.GetRoleScriptData(RoleID,1,RoleDataType["ChongzhuGreen"]) 		 	--��ȡ�����ɫ����������¼

	for green = 8700007,8700012 do
		if role.GetRoleItemNum(RoleID,green) > 0 then												--�������������ɫ����
			local greenUp = green + 6
			local greenDown = green - 6
			local shibaiNum2 = math.floor(ChongzhuGreen/10000)											--��ȡ��ɫ����ʧ�ܴ���
			local bubianNum2 = math.floor(ChongzhuGreen/10)%1000										--��ȡ��ɫ�����������
			local shengjiNum2 = ChongzhuGreen%10														--��ȡ��ɫ�����Ƿ��ֱ������
			if bubianNum2 >= 9 and shibaiNum2 >= 3 or shengjiNum2 >= 1 then							--�������������ڵ���9����ʧ�ܴ������ڵ���3���߿�ֱ������Ϊ1
				role.RemoveFromRole(MapID,InstanceID,RoleID,green,1,420)								--ɾ��������ɫ����
				role.AddRoleItem(MapID, InstanceID,RoleID,greenUp,1,3,8,420)							--�����ɫ����
				role.SetRoleScriptData(RoleID,1,RoleDataType["ChongzhuGreen"],0)						--���м�¼��0
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 0)
				msg.AddMsgEvent(MsgID, 1, 4304)
				msg.DispatchRoleMsgEvent(RoleID,MsgID)
			elseif bubianNum2 < 9 or shibaiNum2 < 3 and shengjiNum2 == 0 then							--����������С��9����ʧ�ܴ���С��3���Ҳ���ֱ������
				local temp2 = math.random(100)
				if temp2 <= 75 then
					role.RemoveFromRole(MapID,InstanceID,RoleID,green,1,420)							--ɾ��������ɫ����
					role.AddRoleItem(MapID,InstanceID,RoleID,green,1,2,8,420)							--�����ɫ����
					role.SetRoleScriptData(RoleID,1,RoleDataType["ChongzhuGreen"],ChongzhuGreen+10)		--��ɫ�������������1
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 4302)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					else
					role.RemoveFromRole(MapID,InstanceID,RoleID,green,1,420)							--ɾ��������ɫ����
					role.AddRoleItem(MapID,InstanceID,RoleID,greenDown,1,1,8,420)						--��������
					role.SetRoleScriptData(RoleID,1,RoleDataType["ChongzhuGreen"],ChongzhuGreen+10000)	--��ɫ����ʧ�ܴ�����1
					role.SetRoleScriptData(RoleID,1,RoleDataType["ChongzhuYellow"],ChongzhuYellow+1)	--������İ�����ֱ������
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 4303)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
			return
		end
	end

	local ChongzhuBlue = role.GetRoleScriptData(RoleID,1,RoleDataType["ChongzhuBlue"]) 		 	--��ȡ�����ɫ����������¼

	for blue = 8700013,8700018 do
		if role.GetRoleItemNum(RoleID,blue) > 0 then												--�������������ɫ����
			local blueUp = blue + 6
			local blueDown = blue - 6
			local shibaiNum3 = math.floor(ChongzhuBlue/10000)											--��ȡ��ɫ����ʧ�ܴ���
			local bubianNum3 = math.floor(ChongzhuBlue/10)%1000										--��ȡ��ɫ�����������
			local shengjiNum3 = ChongzhuBlue%10														--��ȡ��ɫ�����Ƿ��ֱ������
			if bubianNum3 >= 30 and shibaiNum3 >= 10 or shengjiNum3 >= 1 then							--�������������ڵ���15����ʧ�ܴ������ڵ���5���߿�ֱ������Ϊ1
				role.RemoveFromRole(MapID,InstanceID,RoleID,blue,1,420)								--ɾ��������ɫ����
				role.AddRoleItem(MapID, InstanceID,RoleID,blueUp,1,4,8,420)							--��ӳ�ɫ����
				role.SetRoleScriptData(RoleID,1,RoleDataType["ChongzhuBlue"],0)						--���м�¼��0
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 100, 4301)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 4, 6000224)
				msg.AddMsgEvent(MsgID, 4, blue)
				msg.AddMsgEvent(MsgID, 4, blueUp)
				msg.DispatchWorldMsgEvent(MsgID)
			elseif bubianNum3 < 30 or shibaiNum3 < 10 and shengjiNum3 == 0 then							--����������С��15����ʧ�ܴ���С��5���Ҳ���ֱ������
				local temp3 = math.random(100)
				if temp3 <= 75 then
					role.RemoveFromRole(MapID,InstanceID,RoleID,blue,1,420)							--ɾ��������ɫ����
					role.AddRoleItem(MapID,InstanceID,RoleID,blue,1,3,8,420)							--�����ɫ����
					role.SetRoleScriptData(RoleID,1,RoleDataType["ChongzhuBlue"],ChongzhuBlue+10)		--��ɫ�������������1
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 4302)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					else
					role.RemoveFromRole(MapID,InstanceID,RoleID,blue,1,420)							--ɾ��������ɫ����
					role.AddRoleItem(MapID,InstanceID,RoleID,blueDown,1,2,8,420)						--��������
					role.SetRoleScriptData(RoleID,1,RoleDataType["ChongzhuBlue"],ChongzhuBlue+10000)	--��ɫ����ʧ�ܴ�����1
					role.SetRoleScriptData(RoleID,1,RoleDataType["ChongzhuGreen"],ChongzhuGreen+1)	--������İ�����ֱ������
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 4303)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
			return
		end
	end

	local ChongzhuOrange = role.GetRoleScriptData(RoleID,1,RoleDataType["ChongzhuOrange"]) 		 	--��ȡ��ҳ�ɫ����������¼

	for orange = 8700019,8700024 do
		if role.GetRoleItemNum(RoleID,orange) > 0 then												--����������г�ɫ����
			local orangeUp = orange + 6
			local orangeDown = orange - 6
			local shibaiNum4 = math.floor(ChongzhuOrange/10000)											--��ȡ��ɫ����ʧ�ܴ���
			local bubianNum4 = math.floor(ChongzhuOrange/10)%1000										--��ȡ��ɫ�����������
			local shengjiNum4 = ChongzhuOrange%10														--��ȡ��ɫ�����Ƿ��ֱ������
			if bubianNum4 >= 90 and shibaiNum4 >= 30 or shengjiNum4 >= 1 then							--�������������ڵ���15����ʧ�ܴ������ڵ���5���߿�ֱ������Ϊ1
				role.RemoveFromRole(MapID,InstanceID,RoleID,orange,1,420)								--ɾ�����г�ɫ����
				role.AddRoleItem(MapID, InstanceID,RoleID,orangeUp,1,5,8,420)							--�����ɫ����
				role.SetRoleScriptData(RoleID,1,RoleDataType["ChongzhuOrange"],0)						--���м�¼��0
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 100, 4301)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 4, 6000224)
				msg.AddMsgEvent(MsgID, 4, orange)
				msg.AddMsgEvent(MsgID, 4, orangeUp)
				msg.DispatchWorldMsgEvent(MsgID)
			elseif bubianNum4 < 90 or shibaiNum4 < 30 and shengjiNum4 == 0 then							--����������С��45����ʧ�ܴ���С��15���Ҳ���ֱ������
				local temp4 = math.random(100)
				if temp4 <= 75 then
					role.RemoveFromRole(MapID,InstanceID,RoleID,orange,1,420)							--ɾ�����г�ɫ����
					role.AddRoleItem(MapID,InstanceID,RoleID,orange,1,4,8,420)							--��ӳ�ɫ����
					role.SetRoleScriptData(RoleID,1,RoleDataType["ChongzhuOrange"],ChongzhuOrange+10)		--��ɫ�������������1
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 4302)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					else
					role.RemoveFromRole(MapID,InstanceID,RoleID,orange,1,420)							--ɾ�����г�ɫ����
					role.AddRoleItem(MapID,InstanceID,RoleID,orangeDown,1,3,8,420)						--��������
					role.SetRoleScriptData(RoleID,1,RoleDataType["ChongzhuOrange"],ChongzhuOrange+10000)	--��ɫ����ʧ�ܴ�����1
					role.SetRoleScriptData(RoleID,1,RoleDataType["ChongzhuBlue"],ChongzhuBlue+1)	--������İ�����ֱ������
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 4303)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
			return
		end
	end

	for purple = 8700025,8700030 do
		if  role.GetRoleItemNum(RoleID,purple) > 0 then
			role.RemoveFromRole(MapID,InstanceID,RoleID,purple,1,420)								--ɾ�����г�ɫ����
			role.AddRoleItem(MapID, InstanceID,RoleID,purple,1,5,8,420)							--�����ɫ����
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 0)
			msg.AddMsgEvent(MsgID, 1, 4302)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return
		end
	end

end


aux.RegisterItemEvent(6000224, 1, "I6000224_OnUse")


function I6000224_CanUse(MapID, InstanceID, TypeID, RoleID)
    local bRet, bIgnore = 32, false
	local FreeSize = role.GetBagFreeSize(RoleID)
	for i = 8700001,8700030 do
		if 1 <= role.GetRoleItemNum(RoleID,i) then				--�жϱ������Ƿ��а���
			bRet, bIgnore = 0, false
		end
	end
	if FreeSize < 1 then										--�жϱ����ռ��Ƿ���ڵ���1
	    bRet, bIgnore = 40, false
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(6000224, 0, "I6000224_CanUse")
