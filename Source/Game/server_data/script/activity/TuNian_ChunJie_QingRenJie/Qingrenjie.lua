
--RoleDataType["Qingrenjie_Yutu"]	= 163  --��¼ÿ�����ι��������ID
--RoleDataType["Qingrenjie_YutuJiangli"]	= 164  --��¼ÿ����ҵ���ȡ��������

--�����
function Qingrenjie_OnStart(actID)
	if Qingrenjie == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100505)	--���˽�����֮�ʣ�������ף����������˫��˫�ɣ��ճɾ�����
		msg.DispatchBroadcast(MsgID, -1, -1, -1)
		CreatureID_Qingren1 = map.MapCreateCreature(3017298383, -1, 4900720, 765, 10000, 2086)  -- ����ˢ�¹���Ů
		CreatureID_Qingren2 = map.MapCreateCreature(3017298383, -1, 4900722, 765, 10000, 2078)	-- ����ˢԶ��
		CreatureID_Qingren3 = map.MapCreateCreature(3017298895, -1, 4900721, 1064, 6651, 1113)	-- 濾��ˢ����
	end
end
--�����
function Qingrenjie_OnEnd(actID)
	if Qingrenjie == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100506)	--���˽ڻ�Ѿ���������л��λ�Ļ������룬��ף�������������Ҹ�������
		msg.DispatchBroadcast(MsgID, -1, -1, -1)
		map.MapDeleteCreature(3017298383, -1, CreatureID_Qingren1)
		map.MapDeleteCreature(3017298383, -1, CreatureID_Qingren2)
		map.MapDeleteCreature(3017298895, -1, CreatureID_Qingren3)
	end
end
--�������Ƿ��ʹ��
function I2616511_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	if MapID ~= 3017298895 then
		bRet = 32
	end
	return bRet, bIgnore
end
--������ʹ��
function I2616511_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	if MapID == 3017298895 then
		local shanpao = role.GetRoleScriptData(RoleID,1,RoleDataType["Qingrenjie_YutuJiangli"])
		local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
		CreatureID_Yutu = map.MapCreateCreature(3017298895, -1, 4900723, x+1,y,z+1)
		role.SetRoleScriptData(RoleID,1,RoleDataType["Qingrenjie_Yutu"],CreatureID_Yutu)
		cre.SetCreatureScriptData(MapID, InstanceID, CreatureID_Yutu, 1, 1, RoleID)--��¼ι���ߵ�id
		role.SetRoleScriptData(RoleID,1,RoleDataType["Qingrenjie_YutuJiangli"],shanpao+1)--��¼ι����ι����������
	end
end
--��Ʒ���ʹ��
function I2616514_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 2616516, 1, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, RoleID, level*1000, 102)
end
--��Ʒ���ʹ��
function I2616515_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 2616516, 1, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, RoleID, level*1000, 102)
end
--�ж��ź��Ƿ����
function I2616517_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet,bIgnore = 0,false
	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	if MapID ~= 3017298895 or x > 1060 or x < 1040 or z > 1060 or z < 1040 then
		bRet = 43
	end
	return bRet,bIgnore
end
--ʹ���ź�
function I2616517_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	map.MapCreateCreature(3017298895, -1, 1009235, 1050, 6078, 1050)  -- ˢ1009235ɥ��ҹħ
end
--�жϾ�Ʒ����Ƿ�����͸��Է�
function I2616514_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet,bIgnore = 0,false
	local FreeSize = role.GetBagFreeSize(TargetID)
	if role.GetRoleSex(TargetID) ~= 1 then
		bRet = 34
	elseif FreeSize < 1 then
		bRet = 32
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	end
	return bRet, bIgnore
end
--�жϾ�Ʒ����Ƿ�����͸��Է�
function I2616515_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet,bIgnore = 0,false
	local FreeSize = role.GetBagFreeSize(TargetID)
	if role.GetRoleSex(TargetID) ~= 0 then
		bRet = 34
	elseif FreeSize < 1 then
		bRet = 32
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	end
	return bRet, bIgnore
end
--ι�����ӹ���
function Yutu_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--������ID
	local growNum = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)--ι����ι�������
	if RoleID == seedowner then
		if TalkIndex == -1 then
			if growNum < 5 then
				if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 12604)	-- "С���øոճ�������Ҫ���ϸ�������Ҳ�����׳�ɳ���"
					msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 12602)	-- "ι�����ܲ�"
					msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
					msg.AddMsgEvent(MsgID, 1, 12603)	-- "�뿪"
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local cTime = os.time()
					local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3)
					local s = os.difftime(cTime,lTime) --������һ�ζԻ�ʱ��
					if s < 120 then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 12601) -- ��Ҫι�ҳԵ�̫��Ŷ���Զ��˻�ų������ģ�ÿ��ι���ҵļ��һ��Ҫ��2�������ϣ�
						msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
						msg.AddMsgEvent(MsgID, 1, 12603)	-- "�뿪"
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 12605) -- �������ˣ����ֶ��ˣ�
						msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
						msg.AddMsgEvent(MsgID, 1, 12602)	-- "ι�����ܲ�"
						msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
						msg.AddMsgEvent(MsgID, 1, 12603)	-- "�뿪"
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 12606)  -- лл�㣬���������ˣ���Ҫ�ص�������ȥ�ˣ����Ǹ������µ���������£�
				msg.AddMsgEvent(MsgID, 21, 6)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 12607)	-- "���õ�����"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 4 then
			if role.GetRoleItemNum(RoleID, 2616512) > 0 then
				if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
					local lTime = os.time()
					cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3,lTime)
					cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2,growNum+1)
					unit.AddBuff(MapID, InstanceID, TargetID, 5002801, TargetID)
					role.RemoveFromRole(MapID,InstanceID,RoleID,2616512,1,420)
					local k = math.random(100)
					if k <= 15 then
						role.AddRoleItem(MapID,InstanceID,RoleID,2616513,1,-1,8,420)
					end
				else
					local cTime = os.time() --��ǰʱ��
					local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3)
					local s = os.difftime(cTime,lTime) --������һ�ζԻ�ʱ��
					if s >= 120 then  --�������2����
						unit.AddBuff(MapID, InstanceID, TargetID, 5002801, TargetID)
						cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3,cTime)
						cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2,growNum+1)
						role.RemoveFromRole(MapID,InstanceID,RoleID,2616512,1,420)
						local k = math.random(100)
						if k <= 15 then
							role.AddRoleItem(MapID,InstanceID,RoleID,2616513,1,-1,8,420)
						end
					end
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 12610)  --���ˣ���û�к��ܲ��ˣ��Һö�����
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 6 then					--5��ι���������
			local Qingrenjie_YutuJiangli = role.GetRoleScriptData(RoleID,1,RoleDataType["Qingrenjie_YutuJiangli"])
			if Qingrenjie_YutuJiangli < 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 12609)  --�߿����㲻���ҵ����ˣ�
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif role.GetBagFreeSize(RoleID) < 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 12608) --���ı����ռ䲻��!
				msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
				msg.AddMsgEvent(MsgID, 1, 12603)	-- "�뿪"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif role.GetRoleSex(RoleID) == 0 then
				local Qingrenjie_Yutu = role.GetRoleScriptData(RoleID,1,RoleDataType["Qingrenjie_Yutu"])
				role.AddRoleItem(MapID,InstanceID,RoleID,2616514,1,-1,8,420)
				map.MapDeleteCreature(3017298895, -1, Qingrenjie_Yutu)
				role.SetRoleScriptData(RoleID,1,RoleDataType["Qingrenjie_YutuJiangli"],Qingrenjie_YutuJiangli-1)
			elseif role.GetRoleSex(RoleID) == 1 then
				role.AddRoleItem(MapID,InstanceID,RoleID,2616515,1,-1,8,420)
				local Qingrenjie_Yutu = role.GetRoleScriptData(RoleID,1,RoleDataType["Qingrenjie_Yutu"])
				map.MapDeleteCreature(3017298895, -1, Qingrenjie_Yutu)
				role.SetRoleScriptData(RoleID,1,RoleDataType["Qingrenjie_YutuJiangli"],Qingrenjie_YutuJiangli-1)
			end
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 12609)  --�߿����㲻���ҵ����ˣ�
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	end
end
aux.RegisterCreatureEvent(4900723, 7, "Yutu_OnTalk")
aux.RegisterActEvent(160, 2, "Qingrenjie_OnStart")
aux.RegisterActEvent(160, 3, "Qingrenjie_OnEnd")
aux.RegisterItemEvent(2616511, 1, "I2616511_OnUse")
aux.RegisterItemEvent(2616511, 0, "I2616511_CanUse")
aux.RegisterItemEvent(2616514, 0, "I2616514_CanUse")
aux.RegisterItemEvent(2616515, 0, "I2616515_CanUse")
aux.RegisterItemEvent(2616514, 1, "I2616514_OnUse")
aux.RegisterItemEvent(2616515, 1, "I2616515_OnUse")
aux.RegisterItemEvent(2616517, 1, "I2616517_OnUse")
aux.RegisterItemEvent(2616517, 0, "I2616517_CanUse")
