--��ҽ��봥����3312�������ͷ��צ��ID��1004530������ X=897,Y=276,Z=914

--map script data:
--6:���Ѳ��ڻ�ɱ��
--7:����
--8:����
--9:��צ
--10:BOSS
--11:�ý�id
--12:�ٻ�����id

function m14_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	if ObjID == 3325 then
		local Activated1 = map.GetMapScriptData(MapID,InstanceID,1,1) -----����1��ֵΪ0��ˢ��
		if Activated1 == 0 then
			map.SetMapScriptData(MapID,InstanceID,1,1,1)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 959, 150, 767)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 941, 150, 765)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 939, 150, 747)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 962, 150, 746)
		end
--���� 3368
--��A X=488��Y=150��Z=173
--��B X=500��Y=3��Z=178
--��C X=499��Y=3��Z=191
--��D X=488��Y=3��Z=197
--ˢ��������ʿ��ID��1004547�
	elseif ObjID == 3368 then
		local Activated2 = map.GetMapScriptData(MapID,InstanceID,1,2) -----����2��ֵΪ0��ˢ��
		if Activated2 == 0 then
			map.SetMapScriptData(MapID,InstanceID,1,2,1)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 488, 150, 173)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 500, 3, 178)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 499, 3, 191)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 488, 3, 197)
		end
--���� 3400
--��A X=423��Y=2595��Z=696
--��B X=424��Y=2595��Z=682
--��C X=459��Y=2595��Z=699
--��D X=458��Y=2595��Z=682
--ˢ��������ʿ��ID��1004547��
	elseif ObjID == 3400 then
		local Activated3 = map.GetMapScriptData(MapID,InstanceID,1,3) -----����3��ֵΪ0��ˢ��
		if Activated3 == 0 then
			map.SetMapScriptData(MapID,InstanceID,1,3,1)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 423, 2595, 696)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 424, 2595, 682)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 459, 2595, 699)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 458, 2595, 682)
		end
--���� 3401
--��A X=173��Y=150��Z=467
--��B X=166��Y=150��Z=483
--��C X=150��Y=150��Z=483
--��D X=141��Y=150��Z=467
--ˢ��������ʿ��ID��1004547��
	elseif ObjID == 3401 then
		local Activated4 = map.GetMapScriptData(MapID,InstanceID,1,4) -----����4��ֵΪ0��ˢ��
		if Activated4 == 0 then
			map.SetMapScriptData(MapID,InstanceID,1,4,1)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 173, 150, 467)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 166, 150, 483)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 150, 150, 483)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 141, 150, 467)
		end
--���� 3374
--��A X=774��Y=150��Z=204
--��B X=796��Y=150��Z=204
--��C X=777��Y=150��Z=180
--��D X=791��Y=150��Z=180
--ˢ��������ʿ��ID��1004547��
	elseif ObjID == 3374 then
		local Activated5 = map.GetMapScriptData(MapID,InstanceID,1,5) -----����5��ֵΪ0��ˢ��
		if Activated5 == 0 then
			map.SetMapScriptData(MapID,InstanceID,1,5,1)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 774, 150, 204)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 796, 150, 204)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 777, 150, 180)
			map.MapCreateCreature(MapID, InstanceID, 1004547, 791, 150, 180)
		end
	end
end
aux.RegisterMapEvent("m14",6,"m14_OnEnterTrigger")

--���븱���󣬲��ּ����޷�ʹ��
--m14_SkillLimit = {}
--m14_SkillLimit[11005] = true
--m14_SkillLimit[11009] = true
--m14_SkillLimit[11012] = true
--m14_SkillLimit[11014] = true
--m14_SkillLimit[90012] = true

--function m14_CanUseSkill(MapID, InstanceID, SkillTypeID)
--    if m14_SkillLimit[SkillTypeID] then
--       return false
--    end
--    return true
--end
--ע��
--aux.RegisterMapEvent("m14", 21, "m14_CanUseSkill")

--���븱��ʱɾ����ҵ�����buff
function m14_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
		unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end

	local HuanJiao_ID=map.GetMapScriptData(MapID,InstanceID,1,11)
	local YaoJi_ID=map.GetMapScriptData(MapID,InstanceID,1,12)

	if HuanJiao_ID==0 and YaoJi_ID==0 then
		HuanJiao_ID = map.MapCreateCreature(MapID, InstanceID, 1004548, 683, 3226, 707)
		YaoJi_ID = map.MapCreateCreature(MapID, InstanceID, 1004533, 442, 2595, 638)

		unit.AddBuff(MapID, InstanceID, HuanJiao_ID, 9422601, HuanJiao_ID)
		unit.AddBuff(MapID, InstanceID, YaoJi_ID, 9422601, YaoJi_ID)

		map.SetMapScriptData(MapID,InstanceID,1,11,HuanJiao_ID)
		map.SetMapScriptData(MapID,InstanceID,1,12,YaoJi_ID)
	end
end

--ע��
aux.RegisterMapEvent("m14", 2, "m14_OnPlayerEnter")
